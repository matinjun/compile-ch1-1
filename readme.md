[toc]

# 读文件问题

> 读文件时，即使文件在同一目录且存在，仍然读出错误，通过[解答](https://www.daniweb.com/programming/software-development/threads/406729/current-directory)，使用函数`getcwd`，读取当前文件默认工作目录为*E:\code\C++\Compile\ch1\build*，而代码在*E:\code\C++\Compile\ch1*中，因此出现读不出的现象。

最后解决代码为: <br>

```c++
 std::cout << "\nafter read file: " << std::endl;
 auto lines = read_file_lines("../token-operators");
 for(auto line: lines) {
      std::cout << line << std::endl;
 }
```

# c++知识点

## string::length vs string::empty

> 这两个函数，前者针对字符串长度（真正意义上的字符串，以'\0'结尾）；

> 后者针对分配的空间，即使是空串，如果仍然有空间，后面的函数仍会返回false。

## Set in C++ Standard Template Library (STL)

[https://www.geeksforgeeks.org/set-in-cpp-stl/](https://www.geeksforgeeks.org/set-in-cpp-stl/)

## std::string::npos

[https://www.cplusplus.com/reference/string/string/npos/](https://www.cplusplus.com/reference/string/string/npos/%E2%80%B8)

## [std::string::find](http://www.cplusplus.com/reference/string/string/find/)

## std::string::substr

[http://www.cplusplus.com/reference/string/string/substr/](http://www.cplusplus.com/reference/string/string/substr/%E2%80%B8)

## [lambda](https://docs.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp?view=msvc-160#:~:text=In%20C%2B%2B11%20and,an%20argument%20to%20a%20function.)

## [for_each](https://www.geeksforgeeks.org/for_each-loop-c/)

# [remove-comments](https://www.geeksforgeeks.org/remove-comments-given-cc-program/)
