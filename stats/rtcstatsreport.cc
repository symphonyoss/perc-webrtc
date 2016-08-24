/*
 *  Copyright 2016 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "webrtc/api/rtcstatsreport.h"

namespace webrtc {

RTCStatsReport::ConstIterator::ConstIterator(
    const rtc::scoped_refptr<const RTCStatsReport>& report,
    StatsMap::const_iterator it)
    : report_(report),
      it_(it) {
}

RTCStatsReport::ConstIterator::ConstIterator(const ConstIterator&& other)
    : report_(std::move(other.report_)),
      it_(std::move(other.it_)) {
}

RTCStatsReport::ConstIterator::~ConstIterator() {
}

RTCStatsReport::ConstIterator& RTCStatsReport::ConstIterator::operator++() {
  ++it_;
  return *this;
}

RTCStatsReport::ConstIterator& RTCStatsReport::ConstIterator::operator++(int) {
  return ++(*this);
}

const RTCStats& RTCStatsReport::ConstIterator::operator*() const {
  return *it_->second.get();
}

bool RTCStatsReport::ConstIterator::operator==(
    const RTCStatsReport::ConstIterator& other) const {
  return it_ == other.it_;
}

bool RTCStatsReport::ConstIterator::operator!=(
    const RTCStatsReport::ConstIterator& other) const {
  return !(*this == other);
}

rtc::scoped_refptr<RTCStatsReport> RTCStatsReport::Create() {
  return rtc::scoped_refptr<RTCStatsReport>(
      new rtc::RefCountedObject<RTCStatsReport>());
}

RTCStatsReport::RTCStatsReport() {
}

RTCStatsReport::~RTCStatsReport() {
}

bool RTCStatsReport::AddStats(std::unique_ptr<const RTCStats> stats) {
  return !stats_.insert(std::make_pair(std::string(stats->id()),
                        std::move(stats))).second;
}

const RTCStats* RTCStatsReport::operator[](const std::string& id) const {
  StatsMap::const_iterator it = stats_.find(id);
  if (it != stats_.cend())
    return it->second.get();
  return nullptr;
}

RTCStatsReport::ConstIterator RTCStatsReport::begin() const {
  return ConstIterator(rtc::scoped_refptr<const RTCStatsReport>(this),
                       stats_.cbegin());
}

RTCStatsReport::ConstIterator RTCStatsReport::end() const {
  return ConstIterator(rtc::scoped_refptr<const RTCStatsReport>(this),
                       stats_.cend());
}

}  // namespace webrtc