#ifndef METAMODEL_H
#define METAMODEL_H

// Data objects
static const auto cDefaultObjectType = L"";
static const auto cReportType = L"442a8ca0-5250-439e-9dd3-41b9a44122c1";
static const auto cLogEntryType = L"b5c0226f-d71a-4006-b807-42906faab8b3";
static const auto cSettingsType = L"bdd3c743-a2f5-42fa-9f07-70f67c0ad836";
static const auto cStackFrameType = L"ec519858-38bc-4acb-8f73-b4be8beb68c7";
static const auto cLoadedModuleType = L"a4972cf0-ceab-48f6-ba96-dc2317c250ee";
static const auto cTotalIncreaseType = L"8c97e10d-4411-432e-8962-a4c048a226db";

// Sorters
static const auto cDefaultSorterType = L"";
static const auto cReportSorterByAllocationCountType = L"0a33e609-283e-4462-a086-2e678908c515";
static const auto cReportSorterByAllocatedBytesType = L"3d3ab7bb-4f8f-4db3-a9ff-36e067fb37ef";

// Filters
static const auto cDefaultFilterType = L"";
static const auto cReportFilterWithSymbolsType = L"d8b01d39-33cb-40eb-b778-290cfd6eda30";
static const auto cReportFilterWithSourcesType = L"feccc0a0-313f-4c13-8910-36d8ed2c3cce";
static const auto cReportFilterWithPatternType = L"02850d2b-200e-439c-bfcb-5be2f98c0a04";
static const auto cReportFilterWithoutPatternType = L"84abcb2a-9c74-451f-a037-75d5a9e79fe5";
static const auto cReportFilterHideSystemType = L"08796ccf-fc93-4fe6-bf7c-80245a7b66fb";
static const auto cReportFilterLeakedBytesType = L"22951827-4071-4b6f-94d8-b3572e4b8695";
static const auto cReportFilterLeakedNTimesType = L"2ab70445-3981-4c36-abb6-b58bea83374f";
static const auto cReportFilterWithoutSystemModulesType = L"79b80a21-077f-4dfd-a318-0fe35efc359a";
#endif // METAMODEL_H
