/**
 ******************************************************************************
 * @addtogroup UAVObjects OpenPilot UAVObjects
 * @{ 
 * @addtogroup StabilizationStatus StabilizationStatus 
 * @brief Contains status information to control submodules for stabilization.
 *
 * Autogenerated files and functions for StabilizationStatus Object
 *
 * @{ 
 *
 * @file       stabilizationstatus.h
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010-2013.
 * @brief      Implementation of the StabilizationStatus object. This file has been 
 *             automatically generated by the UAVObjectGenerator.
 * 
 * @note       Object definition file: stabilizationstatus.xml. 
 *             This is an automatically generated file.
 *             DO NOT modify manually.
 *
 * @see        The GNU Public License (GPL) Version 3
 *
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef STABILIZATIONSTATUS_H
#define STABILIZATIONSTATUS_H

/* Object constants */
#define STABILIZATIONSTATUS_OBJID 0x38EAB692
#define STABILIZATIONSTATUS_ISSINGLEINST 1
#define STABILIZATIONSTATUS_ISSETTINGS 0
#define STABILIZATIONSTATUS_ISPRIORITY 0
#define STABILIZATIONSTATUS_NUMBYTES sizeof(StabilizationStatusData)

/* Generic interface functions */
int32_t StabilizationStatusInitialize();
UAVObjHandle StabilizationStatusHandle();
void StabilizationStatusSetDefaults(UAVObjHandle obj, uint16_t instId);

typedef struct __attribute__ ((__packed__)) {
    uint8_t Roll;
    uint8_t Pitch;
    uint8_t Yaw;
    uint8_t Thrust;
}  StabilizationStatusOuterLoopData ;
typedef struct __attribute__ ((__packed__)) {
    uint8_t array[4];
}  StabilizationStatusOuterLoopDataArray ;
#define StabilizationStatusOuterLoopToArray( var ) UAVObjectFieldToArray( StabilizationStatusOuterLoopData, var )

typedef struct __attribute__ ((__packed__)) {
    uint8_t Roll;
    uint8_t Pitch;
    uint8_t Yaw;
    uint8_t Thrust;
}  StabilizationStatusInnerLoopData ;
typedef struct __attribute__ ((__packed__)) {
    uint8_t array[4];
}  StabilizationStatusInnerLoopDataArray ;
#define StabilizationStatusInnerLoopToArray( var ) UAVObjectFieldToArray( StabilizationStatusInnerLoopData, var )


/*
 * Packed Object data (unaligned).
 * Should only be used where 4 byte alignment can be guaranteed
 * (eg a single instance on the heap)
 */
typedef struct {
    StabilizationStatusOuterLoopData OuterLoop;
    StabilizationStatusInnerLoopData InnerLoop;
} __attribute__((packed)) StabilizationStatusDataPacked;

/*
 * Packed Object data.
 * Alignment is forced to 4 bytes so as to avoid the potential for CPU usage faults
 * on Cortex M4F during load/store of float UAVO fields
 */
typedef StabilizationStatusDataPacked __attribute__((aligned(4))) StabilizationStatusData;
    
/* Typesafe Object access functions */
static inline int32_t StabilizationStatusGet(StabilizationStatusData *dataOut) { return UAVObjGetData(StabilizationStatusHandle(), dataOut); }
static inline int32_t StabilizationStatusSet(const StabilizationStatusData *dataIn) { return UAVObjSetData(StabilizationStatusHandle(), dataIn); }
static inline int32_t StabilizationStatusInstGet(uint16_t instId, StabilizationStatusData *dataOut) { return UAVObjGetInstanceData(StabilizationStatusHandle(), instId, dataOut); }
static inline int32_t StabilizationStatusInstSet(uint16_t instId, const StabilizationStatusData *dataIn) { return UAVObjSetInstanceData(StabilizationStatusHandle(), instId, dataIn); }
static inline int32_t StabilizationStatusConnectQueue(xQueueHandle queue) { return UAVObjConnectQueue(StabilizationStatusHandle(), queue, EV_MASK_ALL_UPDATES); }
static inline int32_t StabilizationStatusConnectCallback(UAVObjEventCallback cb) { return UAVObjConnectCallback(StabilizationStatusHandle(), cb, EV_MASK_ALL_UPDATES); }
static inline uint16_t StabilizationStatusCreateInstance() { return UAVObjCreateInstance(StabilizationStatusHandle(), &StabilizationStatusSetDefaults); }
static inline void StabilizationStatusRequestUpdate() { UAVObjRequestUpdate(StabilizationStatusHandle()); }
static inline void StabilizationStatusRequestInstUpdate(uint16_t instId) { UAVObjRequestInstanceUpdate(StabilizationStatusHandle(), instId); }
static inline void StabilizationStatusUpdated() { UAVObjUpdated(StabilizationStatusHandle()); }
static inline void StabilizationStatusInstUpdated(uint16_t instId) { UAVObjInstanceUpdated(StabilizationStatusHandle(), instId); }
static inline void StabilizationStatusLogging() { UAVObjLogging(StabilizationStatusHandle()); }
static inline void StabilizationStatusInstLogging(uint16_t instId) { UAVObjInstanceLogging(StabilizationStatusHandle(), instId); }
static inline int32_t StabilizationStatusGetMetadata(UAVObjMetadata *dataOut) { return UAVObjGetMetadata(StabilizationStatusHandle(), dataOut); }
static inline int32_t StabilizationStatusSetMetadata(const UAVObjMetadata *dataIn) { return UAVObjSetMetadata(StabilizationStatusHandle(), dataIn); }
static inline int8_t StabilizationStatusReadOnly() { return UAVObjReadOnly(StabilizationStatusHandle()); }

/* Field OuterLoop information */

// Enumeration options for field OuterLoop
typedef enum {
    STABILIZATIONSTATUS_OUTERLOOP_DIRECT=0,
    STABILIZATIONSTATUS_OUTERLOOP_ATTITUDE=1,
    STABILIZATIONSTATUS_OUTERLOOP_RATTITUDE=2,
    STABILIZATIONSTATUS_OUTERLOOP_WEAKLEVELING=3,
    STABILIZATIONSTATUS_OUTERLOOP_ALTITUDE=4,
    STABILIZATIONSTATUS_OUTERLOOP_ALTITUDEVARIO=5
} StabilizationStatusOuterLoopOptions;

// Array element names for field OuterLoop
typedef enum {
    STABILIZATIONSTATUS_OUTERLOOP_ROLL=0,
    STABILIZATIONSTATUS_OUTERLOOP_PITCH=1,
    STABILIZATIONSTATUS_OUTERLOOP_YAW=2,
    STABILIZATIONSTATUS_OUTERLOOP_THRUST=3
} StabilizationStatusOuterLoopElem;

// Number of elements for field OuterLoop
#define STABILIZATIONSTATUS_OUTERLOOP_NUMELEM 4

/* Field InnerLoop information */

// Enumeration options for field InnerLoop
typedef enum {
    STABILIZATIONSTATUS_INNERLOOP_DIRECT=0,
    STABILIZATIONSTATUS_INNERLOOP_VIRTUALFLYBAR=1,
    STABILIZATIONSTATUS_INNERLOOP_ACRO=2,
    STABILIZATIONSTATUS_INNERLOOP_AXISLOCK=3,
    STABILIZATIONSTATUS_INNERLOOP_RATE=4,
    STABILIZATIONSTATUS_INNERLOOP_CRUISECONTROL=5
} StabilizationStatusInnerLoopOptions;

// Array element names for field InnerLoop
typedef enum {
    STABILIZATIONSTATUS_INNERLOOP_ROLL=0,
    STABILIZATIONSTATUS_INNERLOOP_PITCH=1,
    STABILIZATIONSTATUS_INNERLOOP_YAW=2,
    STABILIZATIONSTATUS_INNERLOOP_THRUST=3
} StabilizationStatusInnerLoopElem;

// Number of elements for field InnerLoop
#define STABILIZATIONSTATUS_INNERLOOP_NUMELEM 4



/* Set/Get functions */
extern void StabilizationStatusOuterLoopSet(StabilizationStatusOuterLoopData *NewOuterLoop);
extern void StabilizationStatusOuterLoopGet(StabilizationStatusOuterLoopData *NewOuterLoop);
extern void StabilizationStatusOuterLoopArraySet(uint8_t *NewOuterLoop);
extern void StabilizationStatusOuterLoopArrayGet(uint8_t *NewOuterLoop);
extern void StabilizationStatusInnerLoopSet(StabilizationStatusInnerLoopData *NewInnerLoop);
extern void StabilizationStatusInnerLoopGet(StabilizationStatusInnerLoopData *NewInnerLoop);
extern void StabilizationStatusInnerLoopArraySet(uint8_t *NewInnerLoop);
extern void StabilizationStatusInnerLoopArrayGet(uint8_t *NewInnerLoop);


#endif // STABILIZATIONSTATUS_H

/**
 * @}
 * @}
 */