/*
 * Copyright (C) 2025 The Android Open Source Project
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
 */

#pragma once

// go/keep-sorted start
#include <audio_utils/clock.h>
#include <chrono>
#include <cstdint>
#include <string>
// go/keep-sorted end

namespace android::audio_utils {

/**
 * Returns the std::string "HH:MM:SS.MSc" from a system_clock
 */
inline std::string formatTime(int64_t systemTime) {
    const auto time_string = audio_utils_time_string_from_ns(systemTime);

    // The time string is 19 characters (including null termination).
    // Example: "03-27 16:47:06.187"
    //           MM DD HH MM SS MS
    // We offset by 6 to get HH:MM:SS.MSc
    //
    return time_string.time + 6; // offset to remove month/day.
}

/**
 * Returns the std::string "HH:MM:SS.MSc" from a system_clock time_point.
 */
inline std::string formatTime(std::chrono::system_clock::time_point t) {
    return formatTime(std::chrono::nanoseconds(t.time_since_epoch()).count());
}

/**
 * Finds the end of the common time prefix.
 *
 * This is as an option to remove the common time prefix to avoid
 * unnecessary duplicated strings.
 *
 * \param time1 a time string
 * \param time2 a time string
 * \return      the position where the common time prefix ends. For abbreviated
 *              printing of time2, offset the character pointer by this position.
 */
inline size_t commonTimePrefixPosition(std::string_view time1, std::string_view time2) {
    const size_t endPos = std::min(time1.size(), time2.size());
    size_t i;

    // Find location of the first mismatch between strings
    for (i = 0; ; ++i) {
        if (i == endPos) {
            return i; // strings match completely to the length of one of the strings.
        }
        if (time1[i] != time2[i]) {
            break;
        }
        if (time1[i] == '\0') {
            return i; // "printed" strings match completely.  No need to check further.
        }
    }

    // Go backwards until we find a delimiter or space.
    for (; i > 0
           && isdigit(time1[i]) // still a number
           && time1[i - 1] != ' '
            ; --i) {
    }
    return i;
}

/**
 * Returns the unique suffix of time2 that isn't present in time1.
 *
 * If time2 is identical to time1, then an empty string_view is returned.
 * This method is used to elide the common prefix when printing times.
 */
inline std::string_view uniqueTimeSuffix(std::string_view time1, std::string_view time2) {
    const size_t pos = commonTimePrefixPosition(time1, time2);
    return time2.substr(pos);
}

} // namespace android::audio_utils
