---
id: reference_tools_webpages_webpages
name: "Web Pages"
kind: manual
breadcrumb: ["Reference","Tools","WebPages","WebPages"]
tags: ["manual","reference","tools","webpages"]
source: "manual/Reference/Tools/WebPages/WebPages.html"
---

# Web Pages

## Overview and Key Concepts

 

You can use web pages to create HTML interfaces in your model. Web pages added to the model are shown in the Toolbox.

 

 

When using the FlexSim Webserver, you can access each web page by name. The default page shown when connecting to a model instance using the webserver is in MAIN:/project/exec/globals/serverinterface/pagetemplates/default. If you name one of your web pages "defaultpage" then your page will be used as the default web server page for the model instead.

 

 

 

## Model Interaction

 

To interact with the model, you should use custom web queries. If you use custom web queries instead of firing FlexSim events directly, then your web page will work properly within the software and when used with the webserver.

 

While developing model web pages, keep in mind that the JavaScript on the page will be executed on the client browser. FlexScript preprocessor code on the page will execute within the FlexSim instance on the server. When developing and using the pages from within the software, the web page code will be applied to the tree and then requested locally using FlexSim's server interface as if the page had been requested by a client web browser using the webserver.

 

 

 

## External Files

 

You can reference external files (such as images, scripts, and style sheets) using the following special keywords at the beginning of the path.

 

- flexsimmodel/
- flexsimweb/
- flexsimdir/
- flexsimmodules/
- flexsimdocuments/
- flexsimhelp/

 

For example, the line below references an image file in the model directory.

 

```
  
```
