/** 
* @brief Function to count elapsed time
*
* Given start time this function count the total elapsed time from the start time.
*
* @param last_counter start time when counter is started in unsigned64
* @return float64
*/
internal_function
F64 timeMicrosecondsElapsed (U64 *last_counter)
{
    U64 freq = SDL_GetPerformanceFrequency();
    U64 new_counter = SDL_GetPerformanceCounter();
    U64 counter_gap = new_counter - (*last_counter);
    F64 time_gap = ((1000000.0 * (F64)counter_gap) / (F64)freq);
    *last_counter = new_counter;
    return time_gap;
}
