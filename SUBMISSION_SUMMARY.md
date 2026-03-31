# Submission Summary for Problem 038 - vector++

## Final Result
**Perfect Score: 100/100**

## Submissions Made

### Submission 1: ID 766847
- **Status:** Compile Error
- **Issue:** Submitted combined vector.hpp + main.cpp, but OJ test harness expects only vector.hpp
- **Lesson:** The OJ has its own test harness that includes vector.hpp

### Submission 2: ID 766852
- **Status:** Accepted
- **Score:** 100/100
- **Performance:**
  - Move Test: "your vector is about the same speed as std::vector" and "your vector is about the same speed as dark::vector"
  - Realloc Test: "your vector is about the same speed as std::vector" but **"your vector is 15430.63% slower than dark::vector"**
- **Implementation:** Used operator new/delete with std::memcpy for trivially copyable types

### Submission 3: ID 766868 (FINAL - BEST)
- **Status:** Accepted
- **Score:** 100/100
- **Performance:**
  - Move Test: "your vector is about the same speed as std::vector" and "your vector is about the same speed as dark::vector"
  - Realloc Test: **"your vector is 15792.39% faster than std::vector"** and **"your vector is about the same speed as dark::vector"**
- **Time:** 21398 ms (compared to 23889 ms in Submission 2)
- **Implementation:** Optimized to use malloc/realloc/free for trivially copyable types

## Key Optimizations

1. **Memory Management Strategy:**
   - For trivially copyable and destructible types: Use `malloc()`, `realloc()`, and `free()`
   - For complex types: Use placement new/delete
   - This allows realloc() to efficiently expand memory without copying when possible

2. **Type Traits:**
   - Used `std::is_trivially_copyable_v<T>` and `std::is_trivially_destructible_v<T>`
   - Compile-time branching with `if constexpr`

3. **Compiler Optimizations:**
   - `-O3` for maximum optimization
   - `-march=native` for CPU-specific optimizations
   - `-mtune=native` for CPU-specific tuning
   - `-ffast-math` for fast floating-point math
   - `-funroll-loops` for loop unrolling
   - `-flto` for link-time optimization

## Comparison to Competition

| Test | vs std::vector | vs dark::vector |
|------|---------------|-----------------|
| Move Test | Same speed | Same speed |
| Realloc Test | **15792.39% faster** | Same speed |

## Conclusion

Successfully implemented a high-performance vector that:
- Achieves perfect 100/100 score
- Matches dark::vector performance (the fastest reference implementation)
- Significantly outperforms std::vector in reallocation scenarios
- Uses only 3 out of 5 allowed submission attempts
