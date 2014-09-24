#ifndef _TCUANDROIDRENDERACTIVITY_HPP
#define _TCUANDROIDRENDERACTIVITY_HPP
/*-------------------------------------------------------------------------
 * drawElements Quality Program Tester Core
 * ----------------------------------------
 *
 * Copyright 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *//*!
 * \file
 * \brief RenderActivity base class.
 *//*--------------------------------------------------------------------*/

#include "tcuDefs.hpp"
#include "tcuAndroidNativeActivity.hpp"
#include "deThread.hpp"
#include "deThreadSafeRingBuffer.hpp"

namespace tcu
{
namespace Android
{

enum MessageType
{
	// Execution control messages. No data argument.
	MESSAGE_RESUME = 0,
	MESSAGE_PAUSE,
	MESSAGE_FINISH,

	// Window messages. Argument is ANativeWindow pointer.
	MESSAGE_WINDOW_CREATED,
	MESSAGE_WINDOW_RESIZED,
	MESSAGE_WINDOW_DESTROYED,

	// Input queue messages. Argument is AInputQueue pointer.
	MESSAGE_INPUT_QUEUE_CREATED,
	MESSAGE_INPUT_QUEUE_DESTROYED,

	MESSAGE_SYNC,					//!< Main thread requests sync. Data argument is de::Semaphore* that needs to be incremented.

	MESSAGETYPE_LAST
};

struct Message
{
	MessageType		type;			//!< Message type.
	void*			data;			//!< Optional data argument.

	Message (void)
		: type	(MESSAGETYPE_LAST)
		, data	(DE_NULL)
	{
	}

	explicit Message (MessageType type_, void* data_ = DE_NULL)
		: type	(type_)
		, data	(data_)
	{
	}
};

enum WindowState
{
	WINDOWSTATE_NOT_CREATED = 0,	//!< Framework hasn't signaled window creation.
	WINDOWSTATE_NOT_INITIALIZED,	//!< Framework hasn't signaled first resize after creation and thus size is not final.
	WINDOWSTATE_READY,				//!< Window is ready for use.
	WINDOWSTATE_DESTROYED,			//!< Window has been destroyed.

	WINDOWSTATE_LAST
};

typedef de::ThreadSafeRingBuffer<Message> MessageQueue;

class RenderThread : private de::Thread
{
public:
							RenderThread				(NativeActivity& activity);
							~RenderThread				(void);

	void					start						(void);
	void					resume						(void);
	void					pause						(void);
	void					destroy						(void);
	void					enqueue						(const Message& message);
	void					sync						(void);

	void					run							(void);

protected:
	virtual void			onInputEvent				(AInputEvent* event) { DE_UNREF(event); }
	virtual void			onWindowCreated				(ANativeWindow* window) = 0;
	virtual void			onWindowResized				(ANativeWindow* window) = 0;
	virtual void			onWindowDestroyed			(ANativeWindow* window) = 0;
	virtual bool			render						(void) = 0;

	NativeActivity&			getNativeActivity			(void) { return m_activity; }

private:
	void					processMessage				(const Message& message);

	// Shared state.
	NativeActivity&			m_activity;
	MessageQueue			m_msgQueue;

	// Parent thread state.
	bool					m_threadRunning;

	// Thread state.
	AInputQueue*			m_inputQueue;
	WindowState				m_windowState;
	ANativeWindow*			m_window;
	bool					m_paused;					//!< Is rendering paused?
	bool					m_finish;					//!< Has thread received FINISH message?
};

class RenderActivity : public NativeActivity
{
public:
							RenderActivity				(ANativeActivity* activity);
	virtual					~RenderActivity				(void);

	virtual void			onStart						(void);
	virtual void			onResume					(void);
	virtual void			onPause						(void);
	virtual void			onStop						(void);
	virtual void			onDestroy					(void);

	virtual void			onNativeWindowCreated		(ANativeWindow* window);
	virtual void			onNativeWindowResized		(ANativeWindow* window);
	virtual void			onNativeWindowRedrawNeeded	(ANativeWindow* window);
	virtual void			onNativeWindowDestroyed		(ANativeWindow* window);

	virtual void			onInputQueueCreated			(AInputQueue* queue);
	virtual void			onInputQueueDestroyed		(AInputQueue* queue);

protected:
	//! Set rendering thread. Must be called at construction time.
	void					setThread					(RenderThread* thread);

private:
							RenderActivity				(const RenderActivity& other);
	RenderActivity&			operator=					(const RenderActivity& other);

	RenderThread*			m_thread;
};

} // Android
} // tcu

#endif // _TCUANDROIDRENDERACTIVITY_HPP