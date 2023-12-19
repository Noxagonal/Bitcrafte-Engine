#pragma once

#include <core/diagnostic/assertion/Assert.hpp>
#include <core/containers/List.hpp>
#include <core/containers/Map.hpp>

#include <functional>



namespace bc {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Event class is a signalling system that can propagate signals to it's listeners.
///
/// Events allow for a multi-listener pattern, where an event can be registered to listen multiple other events, and may
/// delegate the signal to multiple other listeners as well. Event signals can create a tree of sorts where signals
/// are propagated from one event to all of its listeners, and those in turn can propagate it further. This tree is one-way
/// only, so if events in the middle of the tree are signalled, only its listeners are notified.
///
/// Events may be unregistered manually, or in cases where the event is destroyed, it automatically un-registers itself from
/// every event notifying it.
///
/// Each event may have multiple callbacks registered, these callbacks are the primary way of detecting when a signal was
/// observed. Callbacks may be registered ad un-registered at any point in time.
///
/// An event may have template arguments, only events with matching template arguments may be used to signal each other. These
/// template parameters become the arguments passed within the signal, and a matching set of parameters must be provided in the
/// callback function.
///
/// @warning
/// Cyclic signals are not permitted and results in a dead-lock.
///
/// @note
/// When un-registering, listeners of a particular event are not re-routed to listen the events signalling of the event being
/// un-registered.
///
/// @note
/// Callbacks are called before propagating the signal to listeners.
/// 
/// @tparam ...Args
///	Signal parameter types. Can be "<>" if no parameters are needed.
template<typename ...Args>
class Event
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Event() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Event(
		const Event<Args...>							&	other
	) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Event(
		Event<Args...>									&&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~Event()
	{
		for( auto o : listeners ) {
			o->listening_to.Erase( this );
		}
		for( auto o : listening_to ) {
			o->listeners.Erase( this );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Event												&	operator=(
		const Event<Args...>							&	other
	) = delete;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Event												&	operator=(
		Event<Args...>									&&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Add additional event to signal when this event is signalled.
	/// 
	/// @param event
	///	Event to signal when this event is signalled.
	void													RegisterObserver(
		Event<Args...>									*	event
	)
	{
		BAssert( listeners.Find( event ) == listeners.end(), "tried registering same observer twice to the same event" );
		#if BITCRAFTE_DEVELOPMENT_BUILD
		// Returns true if this is found in any of the other events.
		auto FindCircularDependencies = []( Event<Args...> * self, Event<Args...> * event ) -> bool
		{
			auto recursive = [self]( const auto & recursive_func, Event<Args...> * event ) -> bool
			{
				for( auto o : event->listeners ) {
					if( o == self ) return true;
					return recursive_func( recursive_func, o );
				}
				return false;
			};
			return recursive( recursive, event );
		};
		auto FindDoubleDependencies = []( Event<Args...> * self, Event<Args...> * event ) -> bool
		{
			auto recursive = [self]( const auto & recursive_func, Event<Args...> * event ) -> bool
			{
				for( auto o : event->listening_to ) {
					if( o == self ) return true;
					return recursive_func( recursive_func, o );
				}
				return false;
			};
			return recursive( recursive, event );
		};
		BAssert( !FindCircularDependencies( this, event ), "Failed to register an observer to an event, circular registration of listeners" );
		BAssert( !FindDoubleDependencies( this, event ), "Failed to register an observer to an event, double registration of observer" );
		#endif

		listeners.PushBack( event );
		event->listening_to.PushBack( this );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Add additional function to call when this event is signalled.
	///
	/// @see
	/// cb::Event::UnRegisterCallback()
	/// 
	/// @warning
	/// There are no way to automatically unregister callbacks. Be mindful of registering member functions and lambdas that capture
	/// anything as those callbacks will still be called even if the object is destroyed.
	///
	/// @param callback
	///	Function to call when this event is signalled.
	/// 
	/// @return
	/// returns callback id which can be used to unregister this callback.
	uint64_t												RegisterCallback(
		std::function<void( Args... )>						callback
	)
	{
		++callback_counter;
		callbacks.Emplace( callback_counter, callback );
		return callback_counter;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Remove previously registered event so it won't be signalled when this event is signalled.
	/// 
	/// @param event
	///	Previously registered event.
	void													UnRegisterObserver(
		Event<Args...>									*	event
	)
	{
		auto it = listeners.Find( event );
		if( it != listeners.end() ) {
			( *it )->listening_to.Erase( this );
			listeners.Erase( it );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Remove previously registered callback so it won't be called when this event is signalled.
	/// 
	/// @param callback_id
	///	Previously registered id you got from Event::RegisterCallback().
	void													UnRegisterCallback(
		uint64_t											callback_id
	)
	{
		auto it = callbacks.Find( callback_id );
		if( it != callbacks.end() ) {
			callbacks.Erase( it );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Signals this event.
	///
	///	Immediately invokes all registered callbacks and signals all listening observer events. Order is callbacks in order they
	/// were added and then listeners in order they were added.
	/// 
	/// @param ...args
	///	Arguments passed along to all listeners and callbacks as parameters.
	void													Signal(
		Args											&&	...args
	)
	{
		for( auto & c : callbacks ) {
			c.second( std::forward<Args>( args )... );
		}
		for( auto o : listeners ) {
			o->Signal( std::forward<Args>( args )... );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get number of listeners registered to this event.
	///
	/// @return
	/// Number of listeners
	size_t													GetObserverCount()
	{
		return listeners.Size();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get number of other events listening_to this event.
	///
	/// @return
	/// Number of listening_to events.
	size_t													GetObservingCount()
	{
		return listening_to.Size();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	List<Event<Args...>*>									listeners;
	List<Event<Args...>*>									listening_to;
	Map<uint64_t, std::function<void( Args... )>>			callbacks;
	uint64_t												callback_counter		= 0;
};



} // bc
