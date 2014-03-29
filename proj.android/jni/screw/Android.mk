LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := screw_static

LOCAL_MODULE_FILENAME := libscrew

LOCAL_SRC_FILES := facebook/SessionAndroid.cpp \
				   facebook/RequestAndroid.cpp \
				   facebook/Helper.cpp \
				   facebook/DialogAndroid.cpp \
                   ../../../Classes/screw/data/Data.cpp \
                   ../../../Classes/screw/facebook/Facebook.cpp \
                   ../../../Classes/screw/facebook/Dialog.cpp \
                   ../../../Classes/screw/facebook/GraphObject.cpp \
                   ../../../Classes/screw/facebook/Request.cpp \
                   ../../../Classes/screw/facebook/Session.cpp \
                   ../../../Classes/screw/facebook/AppRequests.cpp \
                   ../../../Classes/screw/utils/FileUtils.cpp \
                   ../../../Classes/screw/utils/JsonUtils.cpp \
                   ../../../Classes/screw/utils/ValueUtils.cpp \
                   ../../../Classes/screw/utils/StringUtils.cpp \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../Classes/screw

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
					$(LOCAL_PATH)/../../../cocos2d \
					$(LOCAL_PATH)/../.. 

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_STATIC_LIBRARY)
