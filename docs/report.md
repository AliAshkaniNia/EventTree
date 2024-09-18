# Implementation Steps 
Here are the steps I have taken to solve this challenge.


## Adding Proper Tool Support

I use automatic formatting and static analysis tools (namely `clang-format` and `clang-tidy`) in my projects. The catch here is that if you plan to use these tools, you have to add them as early as possible; otherwise, you have to deal with thousands of warnings. So, in the first step, I added these tools by including a utility cmake library (`utils.cmake`) that I had written before and placed in my projects. This CMake library contains helper functions to utilize formatting, static analysis, testing, and debugging (with sanitizers like `AddressSanitizer`). To use these tools, I first used config files from my previous projects (`.clang-tidy` and `.clang-format`) with one exception: I enabled an extensive list of C++ guidelines in `.clang-tidy` as I am starting from scratch and want to write extra reliable code.

Another important topic to mention is naming conventions. Here are the naming conventions used in this project:

- variables: `snake_case`
- functions: `camelCase`
- class names: `PascalCase`
- namespaces: `camelCase`

## Using `eventpp` library

The crux of this problem lies in implementing a multi-producer, multi-consumer event dispatching system. Given that this is a common functionality, I conducted a search for existing libraries to avoid reinventing the wheel unnecessarily when developing a new library. To structure the overall program, I opted for an available library called [eventpp](https://github.com/wqking/eventpp/). In an effort to adhere to SOLID principles, I designed several interfaces to create a loosely coupled system. Following this phase, I intend to construct a minimal alternative to `eventpp`, focusing on both performance and fairness.

## Implementing `SpecialEventQueue`

After considering the requirements of the problem, I decided to design `SpecialEventQueue` as a minimal alternative to `eventpp` as follows: The `SpecialEventQueue` class is a template-based, thread-safe event queue system that supports multiple event types and handlers. It uses `TBB` concurrent containers for thread safety and implements a round-robin approach in the `processOne()` function to ensure fair event processing. The design incorporates concept-based constraints for type safety and uses a template parameter `QueueType` along with a `QueueConcept` to allow easy integration of different queue implementations.

The benefits of this approach include modularity, flexibility, and easy integration of various queue types. By separating the queue implementation from the event system, it's easy to swap out queue implementations without changing the core event logic. Different event types can use different queue implementations if needed, allowing for optimization based on specific use cases. Any queue that satisfies the `QueueConcept` can be easily integrated into the system, including both custom implementations and standard library containers with appropriate wrappers. This design also allows for performance tuning, as users can choose or implement queue types that best suit their performance requirements, such as lock-free queues for high-concurrency scenarios. Additionally, the modular design enhances testability by making it easier to test individual components and swap in mock objects for testing purposes.

## Validation 

After implementing the core logic, validation of the code was crucial. Consequently, comprehensive tests were developed using the Google Test library, ensuring the correct functionality of `SpecialEventQueue` as the rest of the code is simply _auxiliary_ code around the event queue libraries.

Performance is another critical aspect that requires objective measurement. Consequently, a series of benchmarks were created to demonstrate the system's capabilities, including tests for enqueuing, dispatching, and fairness in handling operations. The benchmarks were executed on an Intel(R) Core(TM) i5-2410M CPU, utilizing `Clang-18.1.8` on Ubuntu 24.04. The results indicate that while `eventpp` exhibits superior performance in emit and dispatch operations, `SpecialEventQueue` demonstrates a more equitable handling of burst scenarios. This was quite predictable as implementing fairness in event processing often comes at the cost of some performance overhead, due to the additional logic required to ensure equal treatment of different event types.


<pre>----------------------------------------------------------------------------------------------------
Benchmark                                          Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------------------
<font color="#4E9A06">BM_Fairness_Queue&lt;NaiveSpecialQueue&gt;      </font><font color="#C4A000">    197406 ns       197171 ns   </font><font color="#06989A">      3596</font> Fairness_Index=0.999968 Processed_A=83 Processed_B=84 Processed_C=83
<font color="#4E9A06">BM_Fairness_Queue&lt;ConcurrentSpecialQueue&gt; </font><font color="#C4A000">    244040 ns       243671 ns   </font><font color="#06989A">      2880</font> Fairness_Index=0.999968 Processed_A=84 Processed_B=83 Processed_C=83
<font color="#4E9A06">BM_Fairness_Eventpp                       </font><font color="#C4A000">    193549 ns       193440 ns   </font><font color="#06989A">      3534</font> Fairness_Index=0.766551 Processed_A=147 Processed_B=63 Processed_C=40
</pre>

<pre>-------------------------------------------------------------------------------------------
Benchmark                                                 Time             CPU   Iterations
-------------------------------------------------------------------------------------------
<font color="#4E9A06">BM_EmitEvents_Queue&lt;NaiveSpecialQueue&gt;/8         </font><font color="#C4A000">       885 ns          885 ns   </font><font color="#06989A">    660681</font>
<font color="#4E9A06">BM_EmitEvents_Queue&lt;NaiveSpecialQueue&gt;/64        </font><font color="#C4A000">       887 ns          886 ns   </font><font color="#06989A">    801095</font>
<font color="#4E9A06">BM_EmitEvents_Queue&lt;NaiveSpecialQueue&gt;/512       </font><font color="#C4A000">       906 ns          905 ns   </font><font color="#06989A">    740042</font>
<font color="#4E9A06">BM_EmitEvents_Queue&lt;NaiveSpecialQueue&gt;/4096      </font><font color="#C4A000">       893 ns          891 ns   </font><font color="#06989A">    777650</font>
<font color="#4E9A06">BM_EmitEvents_Queue&lt;NaiveSpecialQueue&gt;/8192      </font><font color="#C4A000">       919 ns          918 ns   </font><font color="#06989A">    766580</font>
<font color="#4E9A06">BM_EmitEvents_Queue&lt;ConcurrentSpecialQueue&gt;/8    </font><font color="#C4A000">      8275 ns         8267 ns   </font><font color="#06989A">     86718</font>
<font color="#4E9A06">BM_EmitEvents_Queue&lt;ConcurrentSpecialQueue&gt;/64   </font><font color="#C4A000">      8426 ns         8409 ns   </font><font color="#06989A">     85093</font>
<font color="#4E9A06">BM_EmitEvents_Queue&lt;ConcurrentSpecialQueue&gt;/512  </font><font color="#C4A000">      8387 ns         8383 ns   </font><font color="#06989A">     82281</font>
<font color="#4E9A06">BM_EmitEvents_Queue&lt;ConcurrentSpecialQueue&gt;/4096 </font><font color="#C4A000">      8258 ns         8255 ns   </font><font color="#06989A">     82222</font>
<font color="#4E9A06">BM_EmitEvents_Queue&lt;ConcurrentSpecialQueue&gt;/8192 </font><font color="#C4A000">      8334 ns         8333 ns   </font><font color="#06989A">     87079</font>
<font color="#4E9A06">BM_EmitEvents_Eventpp/8                          </font><font color="#C4A000">       369 ns          369 ns   </font><font color="#06989A">   1827890</font>
<font color="#4E9A06">BM_EmitEvents_Eventpp/64                         </font><font color="#C4A000">       347 ns          347 ns   </font><font color="#06989A">   1942106</font>
<font color="#4E9A06">BM_EmitEvents_Eventpp/512                        </font><font color="#C4A000">       357 ns          357 ns   </font><font color="#06989A">   1979116</font>
<font color="#4E9A06">BM_EmitEvents_Eventpp/4096                       </font><font color="#C4A000">       381 ns          381 ns   </font><font color="#06989A">   1729934</font>
<font color="#4E9A06">BM_EmitEvents_Eventpp/8192                       </font><font color="#C4A000">       352 ns          352 ns   </font><font color="#06989A">   1963189</font>
</pre>
  


<pre>-------------------------------------------------------------------------------------------------------
Benchmark                                             Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------------------------------------
<font color="#4E9A06">BM_ResponseTime&lt;NaiveSpecialQueue&gt;/8         </font><font color="#C4A000">     13870 ns        13773 ns   </font><font color="#06989A">     52463</font> Avg_Response_Time_us=9.57214 Max_Response_Time_us=13.433 Min_Response_Time_us=6.32 Processed_Events=7 StdDev_Response_Time_us=2.54189
<font color="#4E9A06">BM_ResponseTime&lt;NaiveSpecialQueue&gt;/64        </font><font color="#C4A000">     62385 ns        62138 ns   </font><font color="#06989A">     10055</font> Avg_Response_Time_us=59.8192 Max_Response_Time_us=75.326 Min_Response_Time_us=45.576 Processed_Events=59 StdDev_Response_Time_us=8.07206
<font color="#4E9A06">BM_ResponseTime&lt;NaiveSpecialQueue&gt;/512       </font><font color="#C4A000">    451993 ns       451769 ns   </font><font color="#06989A">      1503</font> Avg_Response_Time_us=564.299 Max_Response_Time_us=603.806 Min_Response_Time_us=514.633 Processed_Events=507 StdDev_Response_Time_us=22.5369
<font color="#4E9A06">BM_ResponseTime&lt;NaiveSpecialQueue&gt;/4096      </font><font color="#C4A000">   3545843 ns      3541729 ns   </font><font color="#06989A">       197</font> Avg_Response_Time_us=3.60314k Max_Response_Time_us=3.77155k Min_Response_Time_us=3.34995k Processed_Events=4.058k StdDev_Response_Time_us=114.927
<font color="#4E9A06">BM_ResponseTime&lt;NaiveSpecialQueue&gt;/8192      </font><font color="#C4A000">   7015556 ns      7008920 ns   </font><font color="#06989A">        99</font> Avg_Response_Time_us=7.21594k Max_Response_Time_us=7.47759k Min_Response_Time_us=6.81487k Processed_Events=8.131k StdDev_Response_Time_us=151.299
<font color="#4E9A06">BM_ResponseTime&lt;ConcurrentSpecialQueue&gt;/8    </font><font color="#C4A000">     16288 ns        16152 ns   </font><font color="#06989A">     44006</font> Avg_Response_Time_us=43.8456 Max_Response_Time_us=74.67 Min_Response_Time_us=17.177 Processed_Events=8 StdDev_Response_Time_us=18.5815
<font color="#4E9A06">BM_ResponseTime&lt;ConcurrentSpecialQueue&gt;/64   </font><font color="#C4A000">     70944 ns        70745 ns   </font><font color="#06989A">     10097</font> Avg_Response_Time_us=303.938 Max_Response_Time_us=519.004 Min_Response_Time_us=57.547 Processed_Events=60 StdDev_Response_Time_us=127.512
<font color="#4E9A06">BM_ResponseTime&lt;ConcurrentSpecialQueue&gt;/512  </font><font color="#C4A000">    516687 ns       516266 ns   </font><font color="#06989A">      1333</font> Avg_Response_Time_us=2.40804k Max_Response_Time_us=4.11787k Min_Response_Time_us=462.264 Processed_Events=481 StdDev_Response_Time_us=1.00695k
<font color="#4E9A06">BM_ResponseTime&lt;ConcurrentSpecialQueue&gt;/4096 </font><font color="#C4A000">   4074735 ns      4073957 ns   </font><font color="#06989A">       170</font> Avg_Response_Time_us=18.4969k Max_Response_Time_us=32.7536k Min_Response_Time_us=4.19093k Processed_Events=4.077k StdDev_Response_Time_us=8.17403k
<font color="#4E9A06">BM_ResponseTime&lt;ConcurrentSpecialQueue&gt;/8192 </font><font color="#C4A000">   8241527 ns      8240407 ns   </font><font color="#06989A">        85</font> Avg_Response_Time_us=38.8239k Max_Response_Time_us=68.9776k Min_Response_Time_us=8.06718k Processed_Events=8.153k StdDev_Response_Time_us=17.5529k
<font color="#4E9A06">BM_ResponseTime_Eventpp/8                    </font><font color="#C4A000">      9336 ns         9297 ns   </font><font color="#06989A">     72274</font> Avg_Response_Time_us=13.5446 Max_Response_Time_us=14.485 Min_Response_Time_us=12.651 Processed_Events=8 StdDev_Response_Time_us=0.611895
<font color="#4E9A06">BM_ResponseTime_Eventpp/64                   </font><font color="#C4A000">     50292 ns        50265 ns   </font><font color="#06989A">     13657</font> Avg_Response_Time_us=34.1596 Max_Response_Time_us=44.591 Min_Response_Time_us=23.779 Processed_Events=64 StdDev_Response_Time_us=6.18458
<font color="#4E9A06">BM_ResponseTime_Eventpp/512                  </font><font color="#C4A000">    375773 ns       375796 ns   </font><font color="#06989A">      1861</font> Avg_Response_Time_us=259.415 Max_Response_Time_us=339.542 Min_Response_Time_us=179.367 Processed_Events=512 StdDev_Response_Time_us=45.957
<font color="#4E9A06">BM_ResponseTime_Eventpp/4096                 </font><font color="#C4A000">   2945781 ns      2945789 ns   </font><font color="#06989A">       239</font> Avg_Response_Time_us=2.09987k Max_Response_Time_us=2.74508k Min_Response_Time_us=1.45874k Processed_Events=4.096k StdDev_Response_Time_us=370.929
<font color="#4E9A06">BM_ResponseTime_Eventpp/8192                 </font><font color="#C4A000">   6021324 ns      6021194 ns   </font><font color="#06989A">       119</font> Avg_Response_Time_us=4.19061k Max_Response_Time_us=5.47938k Min_Response_Time_us=2.90796k Processed_Events=8.192k StdDev_Response_Time_us=742.887
</pre>

