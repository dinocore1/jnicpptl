

#ifndef JNIFIELD_H_
#define JNIFIELD_H_

#include <jni.h>
#include <string>

class JniObject;

template<bool isStatic>
class JniField_base {

protected:
    const char* mFieldName;
    const char* mFieldSignature;

    JNIEnv* mEnv;
    jobject mInstance;
    jfieldID mCachedFieldID;

    JniObject* mInstanceProxy;

public:
    JniField_base(const char* name, const char* signature)
    : mFieldName(name)
    , mFieldSignature(signature)
    , mEnv(NULL)
    , mInstance(NULL)
    , mCachedFieldID(NULL)
    , mInstanceProxy(NULL)
    {}

    JniField_base(JniObject* proxy, const char* name, const char* signature)
    : mFieldName(name)
    , mFieldSignature(signature)
    , mEnv(NULL)
    , mInstance(NULL)
    , mCachedFieldID(NULL)
    , mInstanceProxy(proxy)
    {}

    void setInstance(JNIEnv* env, jobject instance) {
        mEnv = env;
        mInstance = instance;
    }

    JNIEnv* getJNIEnv();
    jclass getClass();
    jobject getInstance();
    jfieldID getFieldID();
};

template<typename T, bool isStatic>
class JniField : public JniField_base<isStatic> {
    typedef JniField<T, isStatic> SelfT;

public:
    JniField(const char* name, const char* signature)
    : JniField_base<isStatic>(name, signature)
    {}

    JniField(JniObject* proxy, const char* name, const char* signature)
    : JniField_base<isStatic>(proxy, name, signature)
    {}

    T get();
    void set(T t);

    SelfT& operator=(const T& rhs) {
        set(rhs);
        return *this;
    }

    operator const T() {
        return get();
    }

};


#endif /* JNIFIELD_H_ */
