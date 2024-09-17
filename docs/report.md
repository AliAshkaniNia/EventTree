# Implementation Steps 
Here are the steps I have taken to solve this challenge.


## Adding Proper Tool Support

I use automatic formatting and static analysis tools (namely `clang-format` and `clang-tidy`) in my projects. The catch here is that if you plan to use these tools, you have to add them as early as possible; otherwise, you have to deal with thousands of warnings. So, in the first step, I added these tools by including a utility cmake library (`utils.cmake`) that I had written before and placed in my projects. This CMake library contains helper functions to utilize formatting, static analysis, testing, and debugging (with sanitizers like `AddressSanitizer`). To use these tools, I first used config files from my previous projects (`.clang-tidy` and `.clang-format`) with one exception: I enabled an extensive list of C++ guidelines in `.clang-tidy` as I am starting from scratch and want to write extra reliable code.

Another important topic to mention is naming conventions. Here are the naming conventions used in this project:

- variables: `snake_case`
- functions: `camelCase`
- class names: `PascalCase`
- namespaces: `camelCase`

# Using `eventpp` library

The crux of this problem lies in implementing a multi-producer, multi-consumer event dispatching system. Given that this is a common functionality, I conducted a search for existing libraries to avoid reinventing the wheel unnecessarily when developing a new library. To structure the overall program, I opted for an available library called [eventpp](https://github.com/wqking/eventpp/). In an effort to adhere to SOLID principles, I designed several interfaces to create a loosely coupled system. Following this phase, I intend to construct a minimal alternative to `eventpp`, focusing on both performance and fairness.


