# Copyright 2015 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import re

from core import perf_benchmark

from telemetry import benchmark
from telemetry.timeline import tracing_category_filter
from telemetry.web_perf import timeline_based_measurement

import page_sets


class _MemoryBenchmark(perf_benchmark.PerfBenchmark):
  """Base class for timeline based memory benchmarks."""

  def SetExtraBrowserOptions(self, options):
    # TODO(perezju): Temporary workaround to disable periodic memory dumps.
    # See: http://crbug.com/513692
    options.AppendExtraBrowserArgs('--enable-memory-benchmarking')

  def CreateTimelineBasedMeasurementOptions(self):
    # Enable only memory-infra, to get memory dumps, and blink.console, to get
    # the timeline markers used for mapping threads to tabs.
    trace_memory = tracing_category_filter.TracingCategoryFilter(
      filter_string='-*,blink.console,disabled-by-default-memory-infra')
    return timeline_based_measurement.Options(overhead_level=trace_memory)


# TODO(bashi): Workaround for http://crbug.com/532075
# @benchmark.Enabled('android') shouldn't be needed.
@benchmark.Enabled('android')
class MemoryHealthPlan(_MemoryBenchmark):
  """Timeline based benchmark for the Memory Health Plan."""

  page_set = page_sets.MemoryHealthStory

  @classmethod
  def Name(cls):
    return 'memory.memory_health_plan'

  @classmethod
  def ValueCanBeAddedPredicate(cls, value, is_first_result):
    # TODO(perezju): Do not ignore baidu failures http://crbug.com/538143
    return (value.tir_label in ['foreground', 'background']
            and value.name.startswith('memory_')
            and not ('baidu' in value.page.name and value.values is None))


# TODO(bashi): Workaround for http://crbug.com/532075
# @benchmark.Enabled('android') shouldn't be needed.
@benchmark.Enabled('android')
class RendererMemoryBlinkMemoryMobile(_MemoryBenchmark):
  """Timeline based benchmark for measuring memory consumption on mobile
  sites on which blink's memory consumption is relatively high."""

  _RE_RENDERER_VALUES = re.compile('memory_.+_renderer')

  page_set = page_sets.BlinkMemoryMobilePageSet

  def SetExtraBrowserOptions(self, options):
    super(RendererMemoryBlinkMemoryMobile, self).SetExtraBrowserOptions(
        options)
    options.AppendExtraBrowserArgs([
        # TODO(bashi): Temporary workaround for http://crbug.com/461788
        '--no-sandbox',
        # Ignore certs errors because record_wpr cannot handle certs correctly
        # in some cases (e.g. WordPress).
        '--ignore-certificate-errors',
    ])

  @classmethod
  def Name(cls):
    return 'memory.blink_memory_mobile'

  @classmethod
  def ValueCanBeAddedPredicate(cls, value, is_first_result):
    return bool(cls._RE_RENDERER_VALUES.match(value.name))
