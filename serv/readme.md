版本迭代网盘服务端

# 思路

使用centos系列操作系统作为服务端操作系统，如果是文本文件上传到网盘，则使用diff和patch做文件的版本控制，如果是二进制文件则直接使用.gz保存每个版本。

判断是否为二进制文件的方法：如果文件内容里有'\0'则为二进制文件，否则为文本文件。

# 可能的情况

1、新增文件

创建新增文件的操作记录，并且保存文件

2、文本文件更新

使用diff创建补丁文件并且保存，更新保存在服务端的文件，添加操作记录

3、二进制文件更新

保存新二进制文件到服务端，添加操作记录

4、文本文件转换为二进制文件

保存二进制到服务端，添加操作记录

5、二进制文件转换为文本文件

保存文本文件到服务端，添加操作记录

6、删除文件

只是标记文件被删除的状态（因为要可以找回，所以不可能会有真的删除这个动作）

7、修改文件名

会修改对该文件所有的追踪

# 接口

http接口使用apache响应

上传下载接口使用自定义socket服务响应

对于网页端，只能登录、登出、查看目录、查看文件列表、查看文件改动列表、查看文件改动详情、查看文本文件、删除文件，不能下载和上传

对于应用端，支持所有功能

## 统一接口响应结果

示例：

```json
{
    "interface": "/cgi-bin/login",
    "success": true,
    "param": {
        "user": "xxx",
        "password": "xxxxxx"
    },
    "data": {},
    "message": ""
}
```

其中：

键 | 含义
-|-
interface | 接口名字，返回这个方便异步调用接口的程序使用
success | 是否成功
param | 传入参数
data | 结果，不同接口根据需求返回不同的json
message | 消息，如在success为false时提示失败原因

1、登录接口

/cgi-bin/login

参数 | 类型 | 含义
-|-|-
user | string | 用户名
password | string | 登录口令

响应返回cookie，应用端需要保存，并且作为cookie的token字段，除了登录外所有http接口都需要在cookie请求头里放入token字段，用来查询登录状态

响应data字段：

键 | 类型 | 含义 | 值示例
-|-|-|-
token | string | cookie请求头需要 | 1674824810


2、登出接口

/cgi-bin/logout

请求头 | 内容
-|-
cookie | token=xxx（xxx为登录接口响应返回的内容）

3、查看目录接口

/cgi-bin/dir

请求头 | 内容
-|-
cookie | token=xxx（xxx为登录接口响应返回的内容）

4、查看文件列表接口

/cgi-bin/ls

请求头 | 内容
-|-
cookie | token=xxx（xxx为登录接口响应返回的内容）

参数 | 类型 | 含义
-|-|-
pwd | string | 当前路径

5、查看文件改动列表

/cgi-bin/changes

请求头 | 内容
-|-
cookie | token=xxx（xxx为登录接口响应返回的内容）

参数 | 类型 | 含义
-|-|-
file | string | 要查看改动列表的文件名字

6、查看文件改动详情

/cgi-bin/diff

请求头 | 内容
-|-
cookie | token=xxx（xxx为登录接口响应返回的内容）

参数 | 类型 | 含义
-|-|-
commit | string | 提交id

7、查看文本文件

/cgi-bin/cat

请求头 | 内容
-|-
cookie | token=xxx（xxx为登录接口响应返回的内容）

参数 | 类型 | 含义
-|-|-
commit | string | 提交id

8、删除文件

/cgi-bin/rm

请求头 | 内容
-|-
cookie | token=xxx（xxx为登录接口响应返回的内容）

参数 | 类型 | 含义
-|-|-
file | string | 要删除的文件名字

9、修改文件名

/cgi-bin/mv

请求头 | 内容
-|-
cookie | token=xxx（xxx为登录接口响应返回的内容）

参数 | 类型 | 含义
-|-|-
file | string | 要移动的文件名字
new | string | 新文件名

10、查看文件/文件夹是否存在

/cgi-bin/access

请求头 | 内容
-|-
cookie | token=xxx（xxx为登录接口响应返回的内容）

参数 | 类型 | 含义
-|-|-
file | string | 要确认是否存在的文件/文件夹名字
type | string | 类型：file、dir、any

type为file则表示确认文件是否存在，为dir则表示确认文件夹是否存在，为any则确认文件或文件夹是否存在

11、查询登录用户名

/cgi-bin/whoami

请求头 | 内容
-|-
cookie | token=xxx（xxx为登录接口响应返回的内容）

响应data字段：

键 | 类型 | 含义 | 值示例
-|-|-|-
token | string | cookie请求头需要 | 1674824810
user | string | 用户名 | xxx
