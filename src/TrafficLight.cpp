#include <iostream>
#include <random>
#include "TrafficLight.h"
#include <chrono>
#include <thread>

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.

    std::lock_guard<std::mutex> lock_grd(_mtx);

    // emplace back for the TL phase msg
    _queue.emplace_back(msg);
    _condition.notify_one();
}


/* Implementation of class "TrafficLight" */

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

// get the traffic light's _currentPhase private member variable
TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

// set the traffic light's _currentPhase private member variable
void TrafficLight::setCurrentPhase(TrafficLightPhase phase)
{
    _currentPhase = phase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    // use the vector threads from base TrafficLightObject class
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 

    // I googled for methods to generate a random number, and I found this StackOverflow answer that was 
    // quite helpful at introducing some new concepts for unbiased random selection between values.
    // Reference: https://stackoverflow.com/questions/12657962/how-do-i-generate-a-random-number-between-two-variables-that-i-have-stored
    std::random_device seed;
    std::mt19937 rand_gen(seed());
    std::uniform_int_distribution<int> rand_distr(4000, 6000);

    // infinite while loop
    while (true)
    {
        // sleep between 4 and 6 seconds and change traffic light phase to opposite (i.e. red --> green)
        std::this_thread::sleep_for(std::chrono::milliseconds(rand_distr(rand_gen)));

        if (_currentPhase == red)
        {
            // set the TL phase to green
            setCurrentPhase(green);
        }
        else if (_currentPhase == green)
        {
            // set the TL phase to red
            setCurrentPhase(red);
        }
        else
        {
            // phases are not allowed to be neither green nor red - throw exception
            throw std::invalid_argument("TrafficLightPhase can only be red or green!");
        }
        
        // sleep for 1ms as per directions for rubric
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // use move semantics to send rvalue reference of this TrafficLight's phase to the message queue
        _message_queue.send(std::move(TrafficLight::getCurrentPhase()));
}

