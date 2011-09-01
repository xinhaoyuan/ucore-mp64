#include <proc/event.h>
#include <mp.h>
#include <intr.h>
#include <trap/trap.h>

PLS event_pool_s global_pool;

#define EVENT_STATUS_WAIT  0
#define EVENT_STATUS_QUEUE 1

void
event_pool_init(event_pool_t pool,
				event_pool_handler_f remote_touch,
				event_pool_handler_f local_exhaust,
				event_pool_handler_f local_stop)
{
	if (pool == NULL)
		pool = &global_pool;

	pool->head = pool->tail = NULL;

	pool->remote_touch = remote_touch;
	pool->local_exhaust = local_exhaust;
	pool->stop = 0;
	pool->local_stop = local_stop;
}

void
event_open(event_t event, event_pool_t pool, event_handler_f handler, void *private)
{
	if (pool == NULL)
		pool = &global_pool;

	event->pool = pool;
	event->status = EVENT_STATUS_WAIT;
	event->handler = handler;
	event->private = private;
}

int
event_activate(event_t event)
{
	local_irq_save();

	if (event->status == EVENT_STATUS_WAIT)
	{
		event->status = EVENT_STATUS_QUEUE;
		event->next   = NULL;
		event_pool_t pool = event->pool;
		if (pool->tail == NULL)
		{
			pool->head = pool->tail = event;
			pool->remote_touch(pool);
		}
		else
		{
			pool->tail->next = event;
			pool->tail = event;
		}
	}
	
	local_irq_restore();
	return 0;
}

void
event_loop(event_pool_t pool)
{
	if (pool == NULL)
		pool = &global_pool;
	
	while (1)
	{
		event_t event;
		local_irq_save();
		if (pool->head == NULL)
		{
			if (pool->stop)
			{
				local_irq_restore();
				break;
			}
			event = NULL;
		}
		else
		{
			event = pool->head;
			event->status = EVENT_STATUS_WAIT;
			if ((pool->head = pool->head->next) == NULL)
			{
				pool->tail = NULL;
			}
		}
		local_irq_restore();

		if (event)
			event->handler(event);
		else pool->local_exhaust(pool);
	}

	pool->local_stop(pool);
}

void
event_loop_stop(event_pool_t pool)
{
	if (pool == NULL)
		pool = &global_pool;
	/* Need not irq disabled */
	pool->stop = 1;
}
