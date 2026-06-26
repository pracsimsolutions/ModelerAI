# FlexScript API surface — compact index

Auto-extracted from FlexSim's shipped XML docs. 116 classes; 790 active commands; 426 deprecated (hidden).

This is the only catalog page in the cached system prompt. For full details on any symbol use the tools:
- `search_kb(query, kind?, module?)` — BM25 search over the entire index (classes + commands + aliases).
- `read_topic(id, kind?)` — fetch the full body of a single topic.
- `list_commands(group)` — fetch the full table of active commands in a group.
- `list_classes(module)` — fetch the full table of classes in a module.

Prefer FlexScript class methods over the equivalent legacy command (e.g. `Object.location.x` over `xloc()`). Deprecated commands are hidden from this index — to find a deprecated symbol's replacement, call `search_kb(query, include_deprecated: true)`.

## Classes (by module)

**Core** (47): `AGV`, `AStar`, `Agent`, `Animation`, `Array`, `Color`, `Conveyor`, `Coroutine`, `DateTime`, `Delay`, `Dispatcher`, `ElevatorBank`, `Empirical`, `Event`, `EventLog`, `EventResult`, `Group`, `JSON`, `List`, `Map`, `Mat4`, `Math`, `Model`, `Navigator`, `Object`, `PowerPoint`, `RegExResult`, `RegExp`, `StatisticsCollector`, `Storage`, `Table`, `TableValidation`, `TaskExecuter`, `TaskSequence`, `Token`, `Tools`, `TrackedVariable`, `Variant`, `Vec2`, `Vec3`, `Vec4`, `XML`, `double`, `int`, `lambda`, `string`, `treenode`

**AGV** (10): `AGV::AllocatableObject`, `AGV::AllocationPoint`, `AGV::BodyOffset`, `AGV::Connections`, `AGV::Request`, `AGV::RequestState`, `AGV::RoutingConstraint`, `AGV::Trailer`, `AGV::TravelPathSection`, `AGV::TraversalPoint`

**AStar** (5): `AStar::Allocation`, `AStar::Cell`, `AStar::Direction`, `AStar::Navigator`, `AStar::Traveler`

**Agent** (2): `Agent::SocialForceState`, `Agent::System`

**Conveyor** (5): `Conveyor::DecisionPoint`, `Conveyor::Item`, `Conveyor::MergeController`, `Conveyor::Motor`, `Conveyor::SlugBuilder`

**Database** (4): `Database::Connection`, `Database::DataType`, `Database::PreparedStatement`, `Database::ResultSet`

**ElevatorBank** (2): `ElevatorBank::Elevator`, `ElevatorBank::Request`

**Http** (3): `Http::Method`, `Http::Request`, `Http::Response`

**List** (4): `List::BackOrder`, `List::Entry`, `List::PullResult`, `List::PushResult`

**PowerPoint** (6): `PowerPoint::Layout`, `PowerPoint::Presentation`, `PowerPoint::Shape`, `PowerPoint::ShapeType`, `PowerPoint::Slide`, `PowerPoint::SlideSizeType`

**StatisticsCollector** (1): `StatisticsCollector::CollectedData`

**Storage** (9): `Storage::Bay`, `Storage::Item`, `Storage::Item.State`, `Storage::Level`, `Storage::Object`, `Storage::QueryFlags`, `Storage::Slot`, `Storage::Slot.Item`, `Storage::System`

**Table** (1): `Table::IndexType`

**TableValidation** (1): `TableValidation::Context`

**TaskSequence** (1): `TaskSequence::Task`

**USD** (7): `USD::Attribute`, `USD::Prim`, `USD::Property`, `USD::Reference`, `USD::References`, `USD::Relationship`, `USD::Stage`

**XML** (8): `XML::Comment`, `XML::Declaration`, `XML::Document`, `XML::Element`, `XML::Node`, `XML::NodePlacement`, `XML::Text`, `XML::Unknown`

## Commands (by group)

Use `list_commands(group)` to fetch the full table for a group.

| Group | # | Sample command names |
|---|---|---|
| (ungrouped) | 36 | `avi`, `cubemap`, `embedfile`, `mediafile`, `mediapath`, `simexec`, … +30 more |
| Communication | 43 | `clientclose`, `clientconnect`, `clientcreate`, `clientreceive`, `clientsend`, `excelclose`, … +37 more |
| Conversions | 15 | `apchar`, `cm`, `convert`, `days`, `fpm`, `fps`, … +9 more |
| Development | 70 | `addcommand`, `backupfile`, `buildall`, `buildnodeflexscript`, `changedisplay`, `cleannodeflexscript`, … +64 more |
| Drawing | 52 | `bindtexture`, `draw3dtext`, `drawbox`, `drawcolumn`, `drawcube`, `drawcylinder`, … +46 more |
| Events | 18 | `addtoeventtypelegend`, `createevent`, `createtickevent`, `delayevents`, `delayeventsofobject`, `destroyevent`, … +12 more |
| FixedResources | 21 | `evaluatepullcriteria`, `getiteminvolved`, `getitemsendto`, `getitemstate`, `getitemvar`, `holditem`, … +15 more |
| Kinematics and Splines | 23 | `addkinematic`, `deactivatekinematics`, `dynamicsplineall`, `fastspline`, `fastsplineall`, `fastsplineupdate`, … +17 more |
| Language | 13 | `a`, `break`, `case`, `continue`, `default`, `else`, … +7 more |
| Media | 29 | `applyshapefactors`, `autoloadallmedia`, `autoloadimages`, `autoloadshapes`, `autoloadsounds`, `flyactive`, … +23 more |
| Miscellaneous | 48 | `addsystemcontrolleritem`, `commandlineparam`, `contextdragconnection`, `createuserlibrary`, `deleteselected`, `dropuserlibraryobject`, … +42 more |
| Model Execution | 16 | `endreplication`, `fastforward`, `getrunstate`, `go`, `realtime`, `resetexperiment`, … +10 more |
| NetworkNodes | 19 | `closenodeedge`, `getedgedist`, `getedgespeedlimit`, `getnetnode`, `getnetnodespline`, `getnetworkdistance`, … +13 more |
| Object Data | 33 | `assertvariable`, `classobject`, `contentunder`, `emptyfluidobject`, `fluidcontent`, `fluidmaxcontent`, … +27 more |
| Object Functions | 16 | `c`, `debug`, `delayednodefunction`, `eventlisten`, `executefsnode`, `i`, … +10 more |
| Object Ports | 21 | `closeallip`, `closeallop`, `closeip`, `closeop`, `cpcpno`, `inputopen`, … +15 more |
| Output | 19 | `endoffile`, `fileclose`, `fileexists`, `fileopen`, `filereadline`, `fpc`, … +13 more |
| Rack Object | 19 | `rackdrawfilledcell`, `rackdrawvirtualcontent`, `rackgetbaycontent`, `rackgetbayloc`, `rackgetbayofitem`, `rackgetbaysize`, … +13 more |
| Statistics | 65 | `addbundleentry`, `addbundlefield`, `bernoulli`, `beta`, `binomial`, `calculateconfidence`, … +59 more |
| Strings | 22 | `asciistr`, `cdir`, `currentfile`, `documentsdir`, `executestring`, `getdirbrowse`, … +16 more |
| Tables | 2 | `exporttable`, `importtable` |
| TaskExecuter Object | 30 | `addsphere`, `changepersonvisuals`, `createcoordinatedtasksequence`, `dispatchcoordinatedtasksequence`, `distancetotravel`, `drawspheres`, … +24 more |
| Tree Access | 4 | `library`, `maintree`, `model`, `views` |
| Tree Node Data | 28 | `$iter`, `checkdllconnection`, `comparetrees`, `createcoupling`, `enablecode`, `findownerobject`, … +22 more |
| User Interface | 128 | `activateview`, `activedocumentnode`, `activedocumentview`, `activeview`, `addviewtext`, `applicationcommand`, … +122 more |

