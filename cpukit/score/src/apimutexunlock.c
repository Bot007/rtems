/**
 * @file
 *
 * @brief Releases the Specified API Mutex
 *
 * @ingroup ScoreAPIMutex
 */

/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/apimutex.h>
#include <rtems/score/coremuteximpl.h>

void _API_Mutex_Unlock( API_Mutex_Control *the_mutex )
{
  Thread_queue_Context queue_context;
  Thread_Life_state    previous_thread_life_state;
  bool                 restore_thread_life_protection;

  previous_thread_life_state = the_mutex->previous_thread_life_state;
  restore_thread_life_protection = the_mutex->Mutex.nest_count == 1;

  _Thread_queue_Context_initialize( &queue_context, NULL );
  _ISR_lock_ISR_disable( &queue_context.Lock_context );
  _CORE_mutex_Surrender( &the_mutex->Mutex, &queue_context );

  if ( restore_thread_life_protection ) {
    _Thread_Set_life_protection( previous_thread_life_state );
  }
}
