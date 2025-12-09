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

#include <audio_utils/Time.h>

#include <gtest/gtest.h>

using namespace android::audio_utils;

TEST(time_utilities, basic) {
    //basic checks that hold regardless of timezone

    // start of epoch is at 0 seconds
    const auto time0 = formatTime(0);
    EXPECT_TRUE(time0.ends_with(":00.000"));

    // 1 second after epoch is 1 second
    const auto time1 = formatTime(1'000'000'000);
    EXPECT_TRUE(time1.ends_with(":01.000"));

    // validate suffix
    EXPECT_EQ(":01.000", std::string(uniqueTimeSuffix(time0, time1)));
}
