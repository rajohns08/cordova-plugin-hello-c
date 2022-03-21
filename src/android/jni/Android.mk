# Android Makefile

PATH_SEP := /

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

#traverse all the directory and subdirectory
define walk
  $(wildcard $(1)) $(foreach e, $(wildcard $(1)$(PATH_SEP)*), $(call walk, $(e)))
endef

SRC_LIST :=
INCLUDE_LIST :=


################################
# prepare shared lib

LOCAL_MODULE := helloc

# JNI interface files
INCLUDE_LIST += $(LOCAL_PATH)
SRC_LIST += $(wildcard $(LOCAL_PATH)/*.c)

# Cross-platform common files
INCLUDE_LIST += $(LOCAL_PATH)/../../common/
ifeq ($(OS),Windows_NT)
	INCLUDE_LIST += ${shell dir $(LOCAL_PATH)\..\..\common\ /ad /b /s}
else
	INCLUDE_LIST += ${shell find $(LOCAL_PATH)/../../common/ -type d}
endif
SRC_LIST += $(filter %.c, $(call walk, $(LOCAL_PATH)/../../common))


$(info LOCAL_PATH:$(LOCAL_PATH))
$(info SRC_LIST:$(SRC_LIST))
$(info INCLUDE_LIST:$(INCLUDE_LIST))

LOCAL_C_INCLUDES := $(INCLUDE_LIST)
LOCAL_SRC_FILES := $(SRC_LIST:$(LOCAL_PATH)/%=%)
LOCAL_SHARED_LIBRARIES := boringcrypto

LOCAL_CFLAGS += -std=c99
LOCAL_CPPFLAGS := -fblocks
TARGET_PLATFORM := android-27
LOCAL_DISABLE_FATAL_LINKER_WARNINGS := true
LOCAL_LDLIBS += -Wl,--no-warn-shared-textrel
LOCAL_LDFLAGS += -fuse-ld=gold

include $(BUILD_SHARED_LIBRARY)

################################

include $(CLEAR_VARS)
LOCAL_MODULE := boringcrypto
LOCAL_SRC_FILES := ../prebuilt/$(TARGET_ARCH_ABI)/libcrypto.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../prebuilt/include/openssl
include $(PREBUILT_SHARED_LIBRARY)
