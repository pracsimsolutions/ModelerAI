---
id: http__response
name: "Http::Response"
kind: class
module: "Http"
signature: "Http::Response"
aliases: []
tags: ["class", "http-module"]
deprecated: false
replacedBy: null
description: "A class representing the Response to a Http Request."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Http\\Http.Response.xml"
---

# Http::Response

A class representing the Response to a Http Request.

## Methods

### saveToFile

```flexscript
void Http::Response.saveToFile(string filePath)
```

Saves the Http.Response value to a file.

**Parameters:**
- `filePath` *(string)*

Http.Request request;
request.host = "www.flexsim.com";
request.port = 443;
request.useSSL = 1;
request.successCallback = Model.find("Tools/successCallback");
Http.Response response = request.sendAndWait();
response.saveToFile(modeldir() + "flexsim_homepage.html");

## Properties

### data

```flexscript
string Http::Response.data (readonly)
```

Gets the data sent with the request.

### failCallback

```flexscript
treenode Http::Response.failCallback (readonly)
```

Gets the fail callback treenode.

### headers

```flexscript
string Http::Response.headers (readonly)
```

Gets the headers of the response.

### host

```flexscript
string Http::Response.host (readonly)
```

Gets the host used in the request.

### labelProperties

```flexscript
Variant Http::Response.labelProperties (readonly)
```

Use your own named properties to get/set label values.

custom labels on Http.Response objects are inherited from the Http.Request object.

Http.Request request = Http.Request("https://www.flexsim.com/");
request.statusCallback = Model.find("Tools/statusCallback");
request.myProcessor = Model.find("Processor1");
Http.Response response = request.sendAndWait();
print(response.myProcessor); // /Processor1

### method

```flexscript
int Http::Response.method (readonly)
```

Gets the request Http.Method used in the request.

### path

```flexscript
string Http::Response.path (readonly)
```

Gets the resource path used in the request.

### port

```flexscript
int Http::Response.port (readonly)
```

Gets the port the request was sent to.

### receivedBytes

```flexscript
int Http::Response.receivedBytes (readonly)
```

Gets the amount of bytes of the response message received.

### statusCallback

```flexscript
treenode Http::Response.statusCallback (readonly)
```

Gets the status callback treenode.

### statusCode

```flexscript
int Http::Response.statusCode (readonly)
```

Gets the request's Http status code.

Http.Request request = Http.Request("https://www.flexsim.com/");
request.statusCallback = Model.find("Tools/statusCallback");
request.myProcessor = Model.find("Processor1");
request.sendAndWait();
//In the statusCallback node//
Http.Response response = param(1);
print(response.statusCode); // last print should be a 200

### successCallback

```flexscript
treenode Http::Response.successCallback (readonly)
```

Gets the success callback treenode.

### timeout

```flexscript
int Http::Response.timeout (readonly)
```

Gets the request timeout in ms used in the request.

### totalBytes

```flexscript
int Http::Response.totalBytes (readonly)
```

Gets the size of the response message in bytes.

### useSSL

```flexscript
int Http::Response.useSSL (readonly)
```

Gets whether SSL was used in the request.

### value

```flexscript
string Http::Response.value (readonly)
```

Gets the response message to the Http request.

Http.Request request = Http.Request("https://www.flexsim.com/");
request.statusCallback = Model.find("Tools/statusCallback");
request.myProcessor = Model.find("Processor1");
Http.Response response = request.sendAndWait();
print(response.value); //prints the response message

