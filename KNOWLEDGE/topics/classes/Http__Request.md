---
id: http__request
name: "Http::Request"
kind: class
module: "Http"
signature: "Http::Request"
aliases: []
tags: ["class", "http-module"]
deprecated: false
replacedBy: null
description: "A class for creating a Http Request."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Http\\Http.Request.xml"
---

# Http::Request

A class for creating a Http Request.

## Methods

### Http.Request

```flexscript
Http.Request Http::Request.Http.Request(string URL)
```

Creates a Http.Request based on the given url.

**Parameters:**
- `URL` *(string)* — A URL specifying the host, and can include the protocol(Http/Https), the port, and the reference path.

**Returns:** A Http.Request

Http.Request request = Http.Request("https://www.flexsim.com:443/flexsim/");

### send

```flexscript
void Http::Request.send()
```

Send an asynchronous Http request.

Http.Request request;
request.host = "www.flexsim.com";
request.port = 443;
request.useSSL = 1;
request.successCallback = Model.find("Tools/successCallback");
request.send();

### sendAndWait

```flexscript
Http.Response Http::Request.sendAndWait()
```

Send a synchronous Http request.

Http.Request request;
request.host = "www.flexsim.com";
request.port = 443;
request.useSSL = 1;
request.status = Model.find("Tools/statusCallback");
Http.Response response = request.sendAndWait();

## Properties

### data

```flexscript
string Http::Request.data
```

Gets/sets the data being sent with the request.

Http.Request request;
request.host = "www.flexsim.com";
request.data = "name1=value1";

### failCallback

```flexscript
treenode Http::Request.failCallback
```

Specify the fail callback treenode.

Http.Request request;
request.host = "www.flexsim.com";
request.failCallback = Model.find("Tools/failCallback");

### headers

```flexscript
string Http::Request.headers
```

Gets/sets the headers being sent with the request.

If this property is not set, then "Content-Type: application/x-www-form-urlencoded\r\n"
will be passed as the headers.

Http.Request request;
request.host = "127.0.0.1";
request.port = 5000;
request.path = "/v1/prediction";
request.data = JSON.stringify(observation);
request.headers = "Content-Type: application/json\r\n";

### host

```flexscript
string Http::Request.host
```

Gets/sets the host.

Http.Request request;
request.host = "www.flexsim.com";

### labelProperties

```flexscript
Variant Http::Request.labelProperties
```

Use your own named properties to get/set label values.

Custom properties added to Http.Request objects will be accessible with the same name on Http.Response objects created in callbacks
and the return value of sendAndWait().

Http.Request request = Http.Request("https://www.flexsim.com/");
request.statusCallback = Model.find("Tools/statusCallback");
request.myProcessor = Model.find("Processor1");
Http.Response response = request.sendAndWait();
print(response.myProcessor); // /Processor1

### method

```flexscript
int Http::Request.method
```

Specify the request Http.Method.

Http.Request request;
request.host = "www.flexsim.com";
request.method = Http.Method.Get;

### path

```flexscript
string Http::Request.path
```

Gets/sets the resource path.

Http.Request request;
request.host = "www.flexsim.com";
request.path = "flexsim";

### port

```flexscript
int Http::Request.port
```

Gets/sets the port the request is being sent to.

Http.Request request;
request.host = "www.flexsim.com";
request.port = 443;

### statusCallback

```flexscript
treenode Http::Request.statusCallback
```

Specify the status callback treenode.

Http.Request request;
request.host = "www.flexsim.com";
request.statusCallback = Model.find("Tools/statusCallback");

### successCallback

```flexscript
treenode Http::Request.successCallback
```

Specify the success callback treenode.

Http.Request request;
request.host = "www.flexsim.com";
request.successCallback = Model.find("Tools/successCallback");

### timeout

```flexscript
int Http::Request.timeout
```

Specify the request timeout in ms.

Http.Request request;
request.host = "www.flexsim.com";
request.timeout = 1000; // 1 second timeout

### useSSL

```flexscript
int Http::Request.useSSL
```

Specify whether to use SSL if applicable.

Http.Request request;
request.host = "www.flexsim.com";
request.useSSL = 1;

