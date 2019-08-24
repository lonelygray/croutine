# croutine
**a tiny and useful co-routine**
**一个简单协程的实现**

* 功能特点：
   * 不依赖操作系统环境，可以用在 mcs51 单片机里；
   * 只有一个头文件里；
   * 模仿了 C# 的 async/await 使用方式；
* 运行环境：
   * 无限制
* 存在的问题:
   * async 函数不可重入（不同 task 不可调用同一个 async 函数）
   * timed_await 在超时返回后async函数的状态没有被重置。
	
* 使用方法：
   * 定义 async 函数：
   ```
   __async_void foo(...) {
       __co_start();
       ...
       yield;
       ...
       __await(time > 1000)
       ...
       __co_end();
   }
   ```
   * 调用async函数：
   ```
   __async_void bar(...) {
       __co_start();
       ...
       __asleep(0);
       ...
       __timed_await(foo(...), 500);
       ...
       __await(foo(...));
       ...
       __co_end();
   }
   ```
