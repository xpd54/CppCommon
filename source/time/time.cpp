/*!
    \file time.cpp
    \brief Time wrapper implementation
    \author Ivan Shynkarenka
    \date 12.07.2016
    \copyright MIT License
*/

#include "time/time.h"

#include "errors/exceptions.h"

#include <time.h>

namespace CppCommon {

Time::Time(const Timestamp& timestamp)
{
    struct tm result;
    time_t time = timestamp.seconds();
#if defined(_WIN32) || defined(_WIN64)
    if (gmtime_s(&result, &time))
        throwex SystemException("Cannot convert the given timestamp (" + std::to_string(timestamp.total()) + ") to date & time structure!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
    if (gmtime_r(&time, &result) != &result)
        throwex SystemException("Cannot convert the given timestamp (" + std::to_string(timestamp.total()) + ") to date & time structure!");
#endif
    _year = result.tm_year + 1900;
    _month = result.tm_mon + 1;
    _day = result.tm_mday;
    _hour = result.tm_hour;
    _minute = result.tm_min;
    _second = result.tm_sec % 60;
    _millisecond = timestamp.milliseconds() % 1000;
    _microsecond = timestamp.microseconds() % 1000;
    _nanosecond = timestamp.nanoseconds() % 1000;
}

Time::Time(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond, int nanosecond)
{
    if (sizeof(time_t) == 4)
    {
        if ((year < 1970) || (year > 2038))
            throwex ArgumentException("Year value is limited in range from 1970 to 2038!");
    }
    else
    {
        if ((year < 1970) || (year > 3000))
            throwex ArgumentException("Year value is limited in range from 1970 to 3000!");
    }
    if ((month < 1) || (month > 12))
        throwex ArgumentException("Month value is limited in range from 1 to 12!");
    if ((day < 1) || (day > 31))
        throwex ArgumentException("Day value is limited in range from 1 to 31!");
    if ((hour < 0) || (hour > 23))
        throwex ArgumentException("Hour value is limited in range from 0 to 23!");
    if ((minute < 0) || (minute > 59))
        throwex ArgumentException("Minute value is limited in range from 0 to 59!");
    if ((second < 0) || (second > 59))
        throwex ArgumentException("Second value is limited in range from 0 to 59!");
    if ((millisecond < 0) || (millisecond > 999))
        throwex ArgumentException("Millisecond value is limited in range from 0 to 999!");
    if ((microsecond < 0) || (microsecond > 999))
        throwex ArgumentException("Microsecond value is limited in range from 0 to 999!");
    if ((nanosecond < 0) || (nanosecond > 999))
        throwex ArgumentException("Nanosecond value is limited in range from 0 to 999!");

    _year = year;
    _month = month;
    _day = day;
    _hour = hour;
    _minute = minute;
    _second = second;
    _millisecond = millisecond;
    _microsecond = microsecond;
    _nanosecond = nanosecond;
}

UtcTimestamp Time::utcstamp() const
{
    struct tm result;
    result.tm_year = _year - 1900;
    result.tm_mon = _month - 1;
    result.tm_mday = _day;
    result.tm_hour = _hour;
    result.tm_min = _minute;
    result.tm_sec = _second;
    result.tm_isdst = -1;

#if defined(_WIN32) || defined(_WIN64)
    time_t time = _mkgmtime(&result);
#elif defined(unix) || defined(__unix) || defined(__unix__)
    time_t time = timegm(&result);
#endif
    if (time == -1)
        throwex SystemException("Cannot convert date & time to UTC timestamp!");

    return UtcTimestamp(time * 1000000000ull + _millisecond * 1000000ull + _microsecond * 1000ull + _nanosecond);
}

LocalTimestamp Time::localstamp() const
{
    struct tm result;
    result.tm_year = _year - 1900;
    result.tm_mon = _month - 1;
    result.tm_mday = _day;
    result.tm_hour = _hour;
    result.tm_min = _minute;
    result.tm_sec = _second;
    result.tm_isdst = -1;

    time_t time = mktime(&result);
    if (time == -1)
        throwex SystemException("Cannot convert date & time to local timestamp!");

    return LocalTimestamp(time * 1000000000ull + _millisecond * 1000000ull + _microsecond * 1000ull + _nanosecond);
}

UtcTime::UtcTime(const Timestamp& timestamp)
{
    struct tm result;
    time_t time = timestamp.seconds();
#if defined(_WIN32) || defined(_WIN64)
    if (gmtime_s(&result, &time))
        throwex SystemException("Cannot convert the given timestamp (" + std::to_string(timestamp.total()) + ") to UTC date & time structure!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
    if (gmtime_r(&time, &result) != &result)
        throwex SystemException("Cannot convert the given timestamp (" + std::to_string(timestamp.total()) + ") to UTC date & time structure!");
#endif
    _year = result.tm_year + 1900;
    _month = result.tm_mon + 1;
    _day = result.tm_mday;
    _hour = result.tm_hour;
    _minute = result.tm_min;
    _second = result.tm_sec % 60;
    _millisecond = timestamp.milliseconds() % 1000;
    _microsecond = timestamp.microseconds() % 1000;
    _nanosecond = timestamp.nanoseconds() % 1000;
}

LocalTime::LocalTime(const Timestamp& timestamp)
{
    struct tm result;
    time_t time = timestamp.seconds();
#if defined(_WIN32) || defined(_WIN64)
    if (localtime_s(&result, &time))
        throwex SystemException("Cannot convert the given timestamp (" + std::to_string(timestamp.total()) + ") to local date & time structure!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
    if (localtime_r(&time, &result) != &result)
        throwex SystemException("Cannot convert the given timestamp (" + std::to_string(timestamp.total()) + ") to local date & time structure!");
#endif
    _year = result.tm_year + 1900;
    _month = result.tm_mon + 1;
    _day = result.tm_mday;
    _hour = result.tm_hour;
    _minute = result.tm_min;
    _second = result.tm_sec % 60;
    _millisecond = timestamp.milliseconds() % 1000;
    _microsecond = timestamp.microseconds() % 1000;
    _nanosecond = timestamp.nanoseconds() % 1000;
}

} // namespace CppCommon