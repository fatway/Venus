# Venus

### Intro
快捷键启动引导程序。花了两天时间，东拼西凑出来的。

之前用过多款启动辅助程序，如：Launchy、ALTRun、音速启动、吉特盒子，用的最长时间（一直到自己这个程序开发完成之前）是吉特盒子。
刚才这段时间在学习VC，就顺手自己做一个出来，不需要那些花哨的功能，仅仅做引导就够了。


### Version

##### v1.0.2

2015-5-13  
增加随系统自启动选项的勾选实现

##### v1.0.1

2015-5-12  
增加防止多程序启动功能，仅能运行一个实例

##### v1.0.0

2015-5-12  
百度各种功能（遍历文件、解析lnk文件、窗口透明色及隐藏、控件字体颜色及大小），集合进来，完成基本功能框架。

主要功能  
1. 使用Alt+SpaceBar作为热键，主窗体隐藏，热键打开透明色窗体
2. 打开时程序读取目录lnk内的快捷方式文件，供用户输入关键字并索引，采用左匹配方式，回车后运行第一个结果
3. 目录lnk内的快捷方式文件需要用户自行添加
4. 增加以`;(分号)`开头的系统组件引导，如calc、cmd、notepad、taskmgr等
5. 增加打开lnk目录的快捷启动键`;lnk`

### TODO

加入选项功能，如：随系统启动，配置快捷键等