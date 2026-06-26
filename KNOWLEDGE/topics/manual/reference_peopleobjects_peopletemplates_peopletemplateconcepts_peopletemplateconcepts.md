---
id: reference_peopleobjects_peopletemplates_peopletemplateconcepts_peopletemplateconcepts
name: "People Templates"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTemplates","PeopleTemplateConcepts","PeopleTemplateConcepts"]
tags: ["manual","reference","peopleobjects","peopletemplates"]
source: "manual/Reference/PeopleObjects/PeopleTemplates/PeopleTemplateConcepts/PeopleTemplateConcepts.html"
---

# People Templates

## Overview and Key Concepts

 

People Templates provide a quick and easy way to create charts for People-based models. You can use People Templates to display data from a People Statistics Table in an intuitive way. The following charts are a few examples of the available templates:

 

 

 

 

All People Templates are a special kind of Chart Template, in that they use standard FlexSim tools to gather the desired data. They are special because they consist solely of a Calculated Table that is configured to query one or more People Statistics Tables.

 

People Templates provide very general data by default, such as average data for all Person flowitems. However, you will likely need get more specific data. The previous example charts show specific data, rather than the default general data. The first categorizes the original data by type. The second and third show examples of creating and comparing multiple subsets of the original data.

 

In order to create these kind of specific charts, all People Templates expose the Subsets and Partitions tabs of the Query Builder interface for the internal Calculated Table. Subsets allow you to create subsets of the original data, and Partitions allow you to categorize the data.

  

#### Subset Limits

 

All People Templates have a Subsets panel. It is identical to the Subsets tab on the Calculated Table, with one exception: some People Templates will not allow you to make Subsets, if you have a certain number of partitions. Details about this exception are documented with each particular template.

   

#### Partition Limits

 

All People Templates have a Partitions panel. It is identical to the Partitions group box on the Calculated Table, with one exception: some templates will limit the number of Partitions you can have. Details about this exception are documented with each particular template.
