LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
				   screw/facebook/SessionAndroid.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/screw/data/Data.cpp \
                   ../../Classes/screw/facebook/Facebook.cpp \
                   ../../Classes/screw/facebook/GraphObject.cpp \
                   ../../Classes/screw/facebook/Request.cpp \
                   ../../Classes/screw/facebook/Session.cpp \
                   ../../Classes/screw/utils/FileUtils.cpp \
                   ../../Classes/screw/utils/JsonUtils.cpp \
                   ../../Classes/screw/utils/ObjectUtils.cpp \
                   ../../Classes/screw/utils/StringUtils.cpp \
                   

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/.. \
					$(LOCAL_PATH)/../../cocos2d

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,2d)
$(call import-module,audio/android)
$(call import-module,Box2D)
