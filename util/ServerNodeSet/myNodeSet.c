/* WARNING: This is a generated file.
 * Any manual changes will be overwritten. */

#include "myNodeSet.h"

/* providesInputTo - ns=1;i=4001 */

static UA_StatusCode function_myNodeSet_0_begin(UA_Server *server, UA_UInt16 *ns)
{
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_ReferenceTypeAttributes attr = UA_ReferenceTypeAttributes_default;
  attr.inverseName = UA_LOCALIZEDTEXT("", "inputProcidedBy");
  attr.displayName = UA_LOCALIZEDTEXT("", "providesInputTo");
#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS
  attr.description = UA_LOCALIZEDTEXT("", "");
#endif
  attr.writeMask = 0;
  attr.userWriteMask = 0;
  retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_REFERENCETYPE,
                                    UA_NODEID_NUMERIC(ns[1], 4001),
                                    UA_NODEID_NUMERIC(ns[0], 33),
                                    UA_NODEID_NUMERIC(ns[0], 45),
                                    UA_QUALIFIEDNAME(ns[1], "providesInputTo"),
                                    UA_NODEID_NULL,
                                    (const UA_NodeAttributes *)&attr, &UA_TYPES[UA_TYPES_REFERENCETYPEATTRIBUTES], NULL, NULL);
  return retVal;
}

static UA_StatusCode function_myNodeSet_0_finish(UA_Server *server, UA_UInt16 *ns)
{
  return UA_Server_addNode_finish(server,
                                  UA_NODEID_NUMERIC(ns[1], 4001));
}

/* FieldDevice - ns=1;i=1001 */

static UA_StatusCode function_myNodeSet_1_begin(UA_Server *server, UA_UInt16 *ns)
{
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_ObjectTypeAttributes attr = UA_ObjectTypeAttributes_default;
  attr.isAbstract = true;
  attr.displayName = UA_LOCALIZEDTEXT("", "FieldDevice");
#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS
  attr.description = UA_LOCALIZEDTEXT("", "");
#endif
  attr.writeMask = 0;
  attr.userWriteMask = 0;
  retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECTTYPE,
                                    UA_NODEID_NUMERIC(ns[1], 1001),
                                    UA_NODEID_NUMERIC(ns[0], 58),
                                    UA_NODEID_NUMERIC(ns[0], 45),
                                    UA_QUALIFIEDNAME(ns[1], "FieldDevice"),
                                    UA_NODEID_NULL,
                                    (const UA_NodeAttributes *)&attr, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES], NULL, NULL);
  return retVal;
}

static UA_StatusCode function_myNodeSet_1_finish(UA_Server *server, UA_UInt16 *ns)
{
  return UA_Server_addNode_finish(server,
                                  UA_NODEID_NUMERIC(ns[1], 1001));
}

/* Pump - ns=1;i=1002 */

static UA_StatusCode function_myNodeSet_2_begin(UA_Server *server, UA_UInt16 *ns)
{
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_ObjectTypeAttributes attr = UA_ObjectTypeAttributes_default;
  attr.displayName = UA_LOCALIZEDTEXT("", "Pump");
#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS
  attr.description = UA_LOCALIZEDTEXT("", "");
#endif
  attr.writeMask = 0;
  attr.userWriteMask = 0;
  retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_OBJECTTYPE,
                                    UA_NODEID_NUMERIC(ns[1], 1002),
                                    UA_NODEID_NUMERIC(ns[1], 1001),
                                    UA_NODEID_NUMERIC(ns[0], 45),
                                    UA_QUALIFIEDNAME(ns[1], "Pump"),
                                    UA_NODEID_NULL,
                                    (const UA_NodeAttributes *)&attr, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES], NULL, NULL);
  return retVal;
}

static UA_StatusCode function_myNodeSet_2_finish(UA_Server *server, UA_UInt16 *ns)
{
  return UA_Server_addNode_finish(server,
                                  UA_NODEID_NUMERIC(ns[1], 1002));
}

/* ManufacturerName - ns=1;i=6001 */

static UA_StatusCode function_myNodeSet_3_begin(UA_Server *server, UA_UInt16 *ns)
{
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 3;
  attr.accessLevel = 3;
  attr.valueRank = -1;
  attr.dataType = UA_NODEID_NUMERIC(ns[0], 12);
  UA_STACKARRAY(UA_String, variablenode_ns_1_i_6001_variant_DataContents, 1);
  UA_init(variablenode_ns_1_i_6001_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
  UA_Variant_setScalar(&attr.value, variablenode_ns_1_i_6001_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
  attr.displayName = UA_LOCALIZEDTEXT("", "ManufacturerName");
#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS
  attr.description = UA_LOCALIZEDTEXT("", "");
#endif
  attr.writeMask = 0;
  attr.userWriteMask = 0;
  retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
                                    UA_NODEID_NUMERIC(ns[1], 6001),
                                    UA_NODEID_NUMERIC(ns[1], 1001),
                                    UA_NODEID_NUMERIC(ns[0], 47),
                                    UA_QUALIFIEDNAME(ns[1], "ManufacturerName"),
                                    UA_NODEID_NUMERIC(ns[0], 63),
                                    (const UA_NodeAttributes *)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
  retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[1], 6001), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
  return retVal;
}

static UA_StatusCode function_myNodeSet_3_finish(UA_Server *server, UA_UInt16 *ns)
{
  return UA_Server_addNode_finish(server,
                                  UA_NODEID_NUMERIC(ns[1], 6001));
}

/* isOn - ns=1;i=6003 */

static UA_StatusCode function_myNodeSet_4_begin(UA_Server *server, UA_UInt16 *ns)
{
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 3;
  attr.accessLevel = 3;
  attr.valueRank = -1;
  attr.dataType = UA_NODEID_NUMERIC(ns[0], 1);
  UA_STACKARRAY(UA_Boolean, variablenode_ns_1_i_6003_variant_DataContents, 1);
  UA_init(variablenode_ns_1_i_6003_variant_DataContents, &UA_TYPES[UA_TYPES_BOOLEAN]);
  UA_Variant_setScalar(&attr.value, variablenode_ns_1_i_6003_variant_DataContents, &UA_TYPES[UA_TYPES_BOOLEAN]);
  attr.displayName = UA_LOCALIZEDTEXT("", "isOn");
#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS
  attr.description = UA_LOCALIZEDTEXT("", "");
#endif
  attr.writeMask = 0;
  attr.userWriteMask = 0;
  retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
                                    UA_NODEID_NUMERIC(ns[1], 6003),
                                    UA_NODEID_NUMERIC(ns[1], 1002),
                                    UA_NODEID_NUMERIC(ns[0], 47),
                                    UA_QUALIFIEDNAME(ns[1], "isOn"),
                                    UA_NODEID_NUMERIC(ns[0], 63),
                                    (const UA_NodeAttributes *)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
  retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[1], 6003), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
  return retVal;
}

static UA_StatusCode function_myNodeSet_4_finish(UA_Server *server, UA_UInt16 *ns)
{
  return UA_Server_addNode_finish(server,
                                  UA_NODEID_NUMERIC(ns[1], 6003));
}

/* startPump - ns=1;i=7001 */

static UA_StatusCode function_myNodeSet_5_begin(UA_Server *server, UA_UInt16 *ns)
{
#ifdef UA_ENABLE_METHODCALLS
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_MethodAttributes attr = UA_MethodAttributes_default;
  attr.executable = true;
  attr.userExecutable = true;
  attr.displayName = UA_LOCALIZEDTEXT("", "startPump");
#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS
  attr.description = UA_LOCALIZEDTEXT("", "");
#endif
  attr.writeMask = 0;
  attr.userWriteMask = 0;
  retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_METHOD,
                                    UA_NODEID_NUMERIC(ns[1], 7001),
                                    UA_NODEID_NUMERIC(ns[1], 1002),
                                    UA_NODEID_NUMERIC(ns[0], 47),
                                    UA_QUALIFIEDNAME(ns[1], "startPump"),
                                    UA_NODEID_NULL,
                                    (const UA_NodeAttributes *)&attr, &UA_TYPES[UA_TYPES_METHODATTRIBUTES], NULL, NULL);
  retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[1], 7001), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
  return retVal;
#else
  return UA_STATUSCODE_GOOD;
#endif /* UA_ENABLE_METHODCALLS */
}

static UA_StatusCode function_myNodeSet_5_finish(UA_Server *server, UA_UInt16 *ns)
{
#ifdef UA_ENABLE_METHODCALLS
  return UA_Server_addMethodNode_finish(server,
                                        UA_NODEID_NUMERIC(ns[1], 7001), NULL, 0, NULL, 0, NULL);
#else
  return UA_STATUSCODE_GOOD;
#endif /* UA_ENABLE_METHODCALLS */
}

/* OutputArguments - ns=1;i=6005 */

static UA_StatusCode function_myNodeSet_6_begin(UA_Server *server, UA_UInt16 *ns)
{
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 1;
  attr.accessLevel = 1;
  attr.valueRank = 1;
  attr.arrayDimensionsSize = 1;
  attr.arrayDimensions = (UA_UInt32 *)UA_Array_new(1, &UA_TYPES[UA_TYPES_UINT32]);
  attr.arrayDimensions[0] = 1;
  attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);

  struct
  {
    UA_String Name;
    UA_NodeId DataType;
    UA_Int32 ValueRank;
    UA_Int32 ArrayDimensionsSize;
    UA_UInt32 *ArrayDimensions;
    UA_LocalizedText Description;
  } variablenode_ns_1_i_6005_Argument_0_0_struct;
  variablenode_ns_1_i_6005_Argument_0_0_struct.Name = UA_STRING("started");
  variablenode_ns_1_i_6005_Argument_0_0_struct.DataType = UA_NODEID_NUMERIC(ns[0], 1);
  variablenode_ns_1_i_6005_Argument_0_0_struct.ValueRank = (UA_Int32)-1;
  variablenode_ns_1_i_6005_Argument_0_0_struct.ArrayDimensionsSize = 1;
  variablenode_ns_1_i_6005_Argument_0_0_struct.ArrayDimensions = (UA_UInt32 *)UA_malloc(sizeof(UA_UInt32));
  variablenode_ns_1_i_6005_Argument_0_0_struct.ArrayDimensions[0] = (UA_UInt32)0;
  variablenode_ns_1_i_6005_Argument_0_0_struct.Description = UA_LOCALIZEDTEXT("", "");
  UA_ExtensionObject *variablenode_ns_1_i_6005_Argument_0_0 = UA_ExtensionObject_new();
  variablenode_ns_1_i_6005_Argument_0_0->encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
  variablenode_ns_1_i_6005_Argument_0_0->content.encoded.typeId = UA_NODEID_NUMERIC(0, 298);
  UA_ByteString_allocBuffer(&variablenode_ns_1_i_6005_Argument_0_0->content.encoded.body, 32000);
  UA_Byte *posvariablenode_ns_1_i_6005_Argument_0_0 = variablenode_ns_1_i_6005_Argument_0_0->content.encoded.body.data;
  const UA_Byte *endvariablenode_ns_1_i_6005_Argument_0_0 = &variablenode_ns_1_i_6005_Argument_0_0->content.encoded.body.data[32000];
  {
    retVal |= UA_encodeBinary(&variablenode_ns_1_i_6005_Argument_0_0_struct.Name, &UA_TYPES[UA_TYPES_STRING], &posvariablenode_ns_1_i_6005_Argument_0_0, &endvariablenode_ns_1_i_6005_Argument_0_0, NULL, NULL);
    retVal |= UA_encodeBinary(&variablenode_ns_1_i_6005_Argument_0_0_struct.DataType, &UA_TYPES[UA_TYPES_NODEID], &posvariablenode_ns_1_i_6005_Argument_0_0, &endvariablenode_ns_1_i_6005_Argument_0_0, NULL, NULL);
    retVal |= UA_encodeBinary(&variablenode_ns_1_i_6005_Argument_0_0_struct.ValueRank, &UA_TYPES[UA_TYPES_INT32], &posvariablenode_ns_1_i_6005_Argument_0_0, &endvariablenode_ns_1_i_6005_Argument_0_0, NULL, NULL);
    retVal |= UA_encodeBinary(&variablenode_ns_1_i_6005_Argument_0_0_struct.ArrayDimensions[0], &UA_TYPES[UA_TYPES_UINT32], &posvariablenode_ns_1_i_6005_Argument_0_0, &endvariablenode_ns_1_i_6005_Argument_0_0, NULL, NULL);
    retVal |= UA_encodeBinary(&variablenode_ns_1_i_6005_Argument_0_0_struct.Description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], &posvariablenode_ns_1_i_6005_Argument_0_0, &endvariablenode_ns_1_i_6005_Argument_0_0, NULL, NULL);
  }
  size_t variablenode_ns_1_i_6005_Argument_0_0_encOffset = (uintptr_t)(posvariablenode_ns_1_i_6005_Argument_0_0 - variablenode_ns_1_i_6005_Argument_0_0->content.encoded.body.data);
  variablenode_ns_1_i_6005_Argument_0_0->content.encoded.body.length = variablenode_ns_1_i_6005_Argument_0_0_encOffset;
  UA_Byte *variablenode_ns_1_i_6005_Argument_0_0_newBody = (UA_Byte *)UA_malloc(variablenode_ns_1_i_6005_Argument_0_0_encOffset);
  memcpy(variablenode_ns_1_i_6005_Argument_0_0_newBody, variablenode_ns_1_i_6005_Argument_0_0->content.encoded.body.data, variablenode_ns_1_i_6005_Argument_0_0_encOffset);
  UA_Byte *variablenode_ns_1_i_6005_Argument_0_0_oldBody = variablenode_ns_1_i_6005_Argument_0_0->content.encoded.body.data;
  variablenode_ns_1_i_6005_Argument_0_0->content.encoded.body.data = variablenode_ns_1_i_6005_Argument_0_0_newBody;
  UA_free(variablenode_ns_1_i_6005_Argument_0_0_oldBody);

  UA_ExtensionObject variablenode_ns_1_i_6005_variant_DataContents[1];
  variablenode_ns_1_i_6005_variant_DataContents[0] = *variablenode_ns_1_i_6005_Argument_0_0;
  UA_Variant_setArray(&attr.value, &variablenode_ns_1_i_6005_variant_DataContents, (UA_Int32)1, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
  attr.displayName = UA_LOCALIZEDTEXT("", "OutputArguments");
#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS
  attr.description = UA_LOCALIZEDTEXT("", "");
#endif
  attr.writeMask = 0;
  attr.userWriteMask = 0;
  retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
                                    UA_NODEID_NUMERIC(ns[1], 6005),
                                    UA_NODEID_NUMERIC(ns[1], 7001),
                                    UA_NODEID_NUMERIC(ns[0], 46),
                                    UA_QUALIFIEDNAME(ns[0], "OutputArguments"),
                                    UA_NODEID_NUMERIC(ns[0], 68),
                                    (const UA_NodeAttributes *)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
  UA_Array_delete(attr.arrayDimensions, 1, &UA_TYPES[UA_TYPES_UINT32]);

  UA_free(variablenode_ns_1_i_6005_Argument_0_0_struct.ArrayDimensions);
  UA_ExtensionObject_delete(variablenode_ns_1_i_6005_Argument_0_0);
  retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[1], 6005), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
  return retVal;
}

static UA_StatusCode function_myNodeSet_6_finish(UA_Server *server, UA_UInt16 *ns)
{
  return UA_Server_addNode_finish(server,
                                  UA_NODEID_NUMERIC(ns[1], 6005));
}

/* ModelName - ns=1;i=6002 */

static UA_StatusCode function_myNodeSet_7_begin(UA_Server *server, UA_UInt16 *ns)
{
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 3;
  attr.accessLevel = 3;
  attr.valueRank = -1;
  attr.dataType = UA_NODEID_NUMERIC(ns[0], 12);
  UA_STACKARRAY(UA_String, variablenode_ns_1_i_6002_variant_DataContents, 1);
  UA_init(variablenode_ns_1_i_6002_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
  UA_Variant_setScalar(&attr.value, variablenode_ns_1_i_6002_variant_DataContents, &UA_TYPES[UA_TYPES_STRING]);
  attr.displayName = UA_LOCALIZEDTEXT("", "ModelName");
#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS
  attr.description = UA_LOCALIZEDTEXT("", "");
#endif
  attr.writeMask = 0;
  attr.userWriteMask = 0;
  retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
                                    UA_NODEID_NUMERIC(ns[1], 6002),
                                    UA_NODEID_NUMERIC(ns[1], 1001),
                                    UA_NODEID_NUMERIC(ns[0], 47),
                                    UA_QUALIFIEDNAME(ns[1], "ModelName"),
                                    UA_NODEID_NUMERIC(ns[0], 63),
                                    (const UA_NodeAttributes *)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
  retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[1], 6002), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
  return retVal;
}

static UA_StatusCode function_myNodeSet_7_finish(UA_Server *server, UA_UInt16 *ns)
{
  return UA_Server_addNode_finish(server,
                                  UA_NODEID_NUMERIC(ns[1], 6002));
}

/* MotorRPM - ns=1;i=6004 */

static UA_StatusCode function_myNodeSet_8_begin(UA_Server *server, UA_UInt16 *ns)
{
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 3;
  attr.accessLevel = 3;
  attr.valueRank = -1;
  attr.dataType = UA_NODEID_NUMERIC(ns[0], 7);
  UA_STACKARRAY(UA_UInt32, variablenode_ns_1_i_6004_variant_DataContents, 1);
  UA_init(variablenode_ns_1_i_6004_variant_DataContents, &UA_TYPES[UA_TYPES_UINT32]);
  UA_Variant_setScalar(&attr.value, variablenode_ns_1_i_6004_variant_DataContents, &UA_TYPES[UA_TYPES_UINT32]);
  attr.displayName = UA_LOCALIZEDTEXT("", "MotorRPM");
#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS
  attr.description = UA_LOCALIZEDTEXT("", "");
#endif
  attr.writeMask = 0;
  attr.userWriteMask = 0;
  retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
                                    UA_NODEID_NUMERIC(ns[1], 6004),
                                    UA_NODEID_NUMERIC(ns[1], 1002),
                                    UA_NODEID_NUMERIC(ns[0], 47),
                                    UA_QUALIFIEDNAME(ns[1], "MotorRPM"),
                                    UA_NODEID_NUMERIC(ns[0], 63),
                                    (const UA_NodeAttributes *)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
  retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[1], 6004), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
  return retVal;
}

static UA_StatusCode function_myNodeSet_8_finish(UA_Server *server, UA_UInt16 *ns)
{
  return UA_Server_addNode_finish(server,
                                  UA_NODEID_NUMERIC(ns[1], 6004));
}

/* stopPump - ns=1;i=7002 */

static UA_StatusCode function_myNodeSet_9_begin(UA_Server *server, UA_UInt16 *ns)
{
#ifdef UA_ENABLE_METHODCALLS
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_MethodAttributes attr = UA_MethodAttributes_default;
  attr.executable = true;
  attr.userExecutable = true;
  attr.displayName = UA_LOCALIZEDTEXT("", "stopPump");
#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS
  attr.description = UA_LOCALIZEDTEXT("", "");
#endif
  attr.writeMask = 0;
  attr.userWriteMask = 0;
  retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_METHOD,
                                    UA_NODEID_NUMERIC(ns[1], 7002),
                                    UA_NODEID_NUMERIC(ns[1], 1002),
                                    UA_NODEID_NUMERIC(ns[0], 47),
                                    UA_QUALIFIEDNAME(ns[1], "stopPump"),
                                    UA_NODEID_NULL,
                                    (const UA_NodeAttributes *)&attr, &UA_TYPES[UA_TYPES_METHODATTRIBUTES], NULL, NULL);
  retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[1], 7002), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
  return retVal;
#else
  return UA_STATUSCODE_GOOD;
#endif /* UA_ENABLE_METHODCALLS */
}

static UA_StatusCode function_myNodeSet_9_finish(UA_Server *server, UA_UInt16 *ns)
{
#ifdef UA_ENABLE_METHODCALLS
  return UA_Server_addMethodNode_finish(server,
                                        UA_NODEID_NUMERIC(ns[1], 7002), NULL, 0, NULL, 0, NULL);
#else
  return UA_STATUSCODE_GOOD;
#endif /* UA_ENABLE_METHODCALLS */
}

/* OutputArguments - ns=1;i=6006 */

static UA_StatusCode function_myNodeSet_10_begin(UA_Server *server, UA_UInt16 *ns)
{
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 1;
  attr.accessLevel = 1;
  attr.valueRank = 1;
  attr.arrayDimensionsSize = 1;
  attr.arrayDimensions = (UA_UInt32 *)UA_Array_new(1, &UA_TYPES[UA_TYPES_UINT32]);
  attr.arrayDimensions[0] = 1;
  attr.dataType = UA_NODEID_NUMERIC(ns[0], 296);

  struct
  {
    UA_String Name;
    UA_NodeId DataType;
    UA_Int32 ValueRank;
    UA_Int32 ArrayDimensionsSize;
    UA_UInt32 *ArrayDimensions;
    UA_LocalizedText Description;
  } variablenode_ns_1_i_6006_Argument_0_0_struct;
  variablenode_ns_1_i_6006_Argument_0_0_struct.Name = UA_STRING("stopped");
  variablenode_ns_1_i_6006_Argument_0_0_struct.DataType = UA_NODEID_NUMERIC(ns[0], 1);
  variablenode_ns_1_i_6006_Argument_0_0_struct.ValueRank = (UA_Int32)-1;
  variablenode_ns_1_i_6006_Argument_0_0_struct.ArrayDimensionsSize = 1;
  variablenode_ns_1_i_6006_Argument_0_0_struct.ArrayDimensions = (UA_UInt32 *)UA_malloc(sizeof(UA_UInt32));
  variablenode_ns_1_i_6006_Argument_0_0_struct.ArrayDimensions[0] = (UA_UInt32)0;
  variablenode_ns_1_i_6006_Argument_0_0_struct.Description = UA_LOCALIZEDTEXT("", "");
  UA_ExtensionObject *variablenode_ns_1_i_6006_Argument_0_0 = UA_ExtensionObject_new();
  variablenode_ns_1_i_6006_Argument_0_0->encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
  variablenode_ns_1_i_6006_Argument_0_0->content.encoded.typeId = UA_NODEID_NUMERIC(0, 298);
  UA_ByteString_allocBuffer(&variablenode_ns_1_i_6006_Argument_0_0->content.encoded.body, 32000);
  UA_Byte *posvariablenode_ns_1_i_6006_Argument_0_0 = variablenode_ns_1_i_6006_Argument_0_0->content.encoded.body.data;
  const UA_Byte *endvariablenode_ns_1_i_6006_Argument_0_0 = &variablenode_ns_1_i_6006_Argument_0_0->content.encoded.body.data[32000];
  {
    retVal |= UA_encodeBinary(&variablenode_ns_1_i_6006_Argument_0_0_struct.Name, &UA_TYPES[UA_TYPES_STRING], &posvariablenode_ns_1_i_6006_Argument_0_0, &endvariablenode_ns_1_i_6006_Argument_0_0, NULL, NULL);
    retVal |= UA_encodeBinary(&variablenode_ns_1_i_6006_Argument_0_0_struct.DataType, &UA_TYPES[UA_TYPES_NODEID], &posvariablenode_ns_1_i_6006_Argument_0_0, &endvariablenode_ns_1_i_6006_Argument_0_0, NULL, NULL);
    retVal |= UA_encodeBinary(&variablenode_ns_1_i_6006_Argument_0_0_struct.ValueRank, &UA_TYPES[UA_TYPES_INT32], &posvariablenode_ns_1_i_6006_Argument_0_0, &endvariablenode_ns_1_i_6006_Argument_0_0, NULL, NULL);
    retVal |= UA_encodeBinary(&variablenode_ns_1_i_6006_Argument_0_0_struct.ArrayDimensions[0], &UA_TYPES[UA_TYPES_UINT32], &posvariablenode_ns_1_i_6006_Argument_0_0, &endvariablenode_ns_1_i_6006_Argument_0_0, NULL, NULL);
    retVal |= UA_encodeBinary(&variablenode_ns_1_i_6006_Argument_0_0_struct.Description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], &posvariablenode_ns_1_i_6006_Argument_0_0, &endvariablenode_ns_1_i_6006_Argument_0_0, NULL, NULL);
  }
  size_t variablenode_ns_1_i_6006_Argument_0_0_encOffset = (uintptr_t)(posvariablenode_ns_1_i_6006_Argument_0_0 - variablenode_ns_1_i_6006_Argument_0_0->content.encoded.body.data);
  variablenode_ns_1_i_6006_Argument_0_0->content.encoded.body.length = variablenode_ns_1_i_6006_Argument_0_0_encOffset;
  UA_Byte *variablenode_ns_1_i_6006_Argument_0_0_newBody = (UA_Byte *)UA_malloc(variablenode_ns_1_i_6006_Argument_0_0_encOffset);
  memcpy(variablenode_ns_1_i_6006_Argument_0_0_newBody, variablenode_ns_1_i_6006_Argument_0_0->content.encoded.body.data, variablenode_ns_1_i_6006_Argument_0_0_encOffset);
  UA_Byte *variablenode_ns_1_i_6006_Argument_0_0_oldBody = variablenode_ns_1_i_6006_Argument_0_0->content.encoded.body.data;
  variablenode_ns_1_i_6006_Argument_0_0->content.encoded.body.data = variablenode_ns_1_i_6006_Argument_0_0_newBody;
  UA_free(variablenode_ns_1_i_6006_Argument_0_0_oldBody);

  UA_ExtensionObject variablenode_ns_1_i_6006_variant_DataContents[1];
  variablenode_ns_1_i_6006_variant_DataContents[0] = *variablenode_ns_1_i_6006_Argument_0_0;
  UA_Variant_setArray(&attr.value, &variablenode_ns_1_i_6006_variant_DataContents, (UA_Int32)1, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
  attr.displayName = UA_LOCALIZEDTEXT("", "OutputArguments");
#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS
  attr.description = UA_LOCALIZEDTEXT("", "");
#endif
  attr.writeMask = 0;
  attr.userWriteMask = 0;
  retVal |= UA_Server_addNode_begin(server, UA_NODECLASS_VARIABLE,
                                    UA_NODEID_NUMERIC(ns[1], 6006),
                                    UA_NODEID_NUMERIC(ns[1], 7002),
                                    UA_NODEID_NUMERIC(ns[0], 46),
                                    UA_QUALIFIEDNAME(ns[0], "OutputArguments"),
                                    UA_NODEID_NUMERIC(ns[0], 68),
                                    (const UA_NodeAttributes *)&attr, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
  UA_Array_delete(attr.arrayDimensions, 1, &UA_TYPES[UA_TYPES_UINT32]);

  UA_free(variablenode_ns_1_i_6006_Argument_0_0_struct.ArrayDimensions);
  UA_ExtensionObject_delete(variablenode_ns_1_i_6006_Argument_0_0);
  retVal |= UA_Server_addReference(server, UA_NODEID_NUMERIC(ns[1], 6006), UA_NODEID_NUMERIC(ns[0], 37), UA_EXPANDEDNODEID_NUMERIC(ns[0], 78), true);
  return retVal;
}

static UA_StatusCode function_myNodeSet_10_finish(UA_Server *server, UA_UInt16 *ns)
{
  return UA_Server_addNode_finish(server,
                                  UA_NODEID_NUMERIC(ns[1], 6006));
}

UA_StatusCode myNodeSet(UA_Server *server)
{
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  /* Use namespace ids generated by the server */
  UA_UInt16 ns[2];
  ns[0] = UA_Server_addNamespace(server, "http://opcfoundation.org/UA/");
  ns[1] = UA_Server_addNamespace(server, "http://yourorganisation.org/example_nodeset/");
  retVal |= function_myNodeSet_0_begin(server, ns);
  retVal |= function_myNodeSet_1_begin(server, ns);
  retVal |= function_myNodeSet_2_begin(server, ns);
  retVal |= function_myNodeSet_3_begin(server, ns);
  retVal |= function_myNodeSet_4_begin(server, ns);
  retVal |= function_myNodeSet_5_begin(server, ns);
  retVal |= function_myNodeSet_6_begin(server, ns);
  retVal |= function_myNodeSet_7_begin(server, ns);
  retVal |= function_myNodeSet_8_begin(server, ns);
  retVal |= function_myNodeSet_9_begin(server, ns);
  retVal |= function_myNodeSet_10_begin(server, ns);
  retVal |= function_myNodeSet_10_finish(server, ns);
  retVal |= function_myNodeSet_9_finish(server, ns);
  retVal |= function_myNodeSet_8_finish(server, ns);
  retVal |= function_myNodeSet_7_finish(server, ns);
  retVal |= function_myNodeSet_6_finish(server, ns);
  retVal |= function_myNodeSet_5_finish(server, ns);
  retVal |= function_myNodeSet_4_finish(server, ns);
  retVal |= function_myNodeSet_3_finish(server, ns);
  retVal |= function_myNodeSet_2_finish(server, ns);
  retVal |= function_myNodeSet_1_finish(server, ns);
  retVal |= function_myNodeSet_0_finish(server, ns);
  return retVal;
}
