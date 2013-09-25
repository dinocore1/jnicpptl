

#ifndef JNIFIELD_H_
#define JNIFIELD_H_

#include <jni.h>

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
    {}

    JniField_base(JniObject* proxy, const char* name, const char* signature);

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

    const T& get();
    SelfT& operator=(const T& rhs);
    operator T();  

};

/*

template <typename NativeT, bool isStatic>
class JniField {
    typedef JniField<NativeT, isStatic> Self;
    
    const char* m_fieldName;
    const char* m_fieldSignature;

    JNIEnv* mEnv;
    jobject mInstance;
    
    
public:

	JniField(const char* name, const char* signature)
    : m_fieldName(name)
    , m_fieldSignature(signature)
    {}

    JniField()

    
    Self& operator=(const NativeT& rhs) {
        set(rhs);
        return *this;
    }
    
    NativeT get() const {
        NativeT result;
        get(result);
        return result;
    }

    jobject getInstance() {
        return mInstance;
    }
    
private:
    void get(NativeT&) const;
    void set(const NativeT&);
    
    jfieldID getFieldID() const {
        // It's not a valid optimization to cache field ids in face of class unloading.
        // We could keep a global reference to the class to prevent it being unloaded, but that seems unfriendly.
        jfieldID result = isStatic ? mEnv->GetStaticFieldID(getObjectClass(), m_fieldName, m_fieldSignature) 
        	: mEnv->GetFieldID(getObjectClass(), m_fieldName, m_fieldSignature);
        if (result == 0) {
				mEnv->ExceptionClear();
                JniException::throwErrorNow(mEnv, "unable to find %s. (%s)", m_fieldName, m_fieldSignature);
				
        }
        return result;
    }
    
    jclass getObjectClass() const {
        // The JNI specification (http://java.sun.com/j2se/1.5.0/docs/guide/jni/spec/functions.html) suggests that GetObjectClass can't fail, so we don't need to check for exceptions.
        return mEnv->GetObjectClass(mInstance);
    }
};

#define JniField_ACCESSORS(TYPE, FUNCTION_NAME_FRAGMENT) \
    template <> void JniField<TYPE, true>::set(const TYPE& rhs) { \
        mEnv->SetStatic ## FUNCTION_NAME_FRAGMENT ## Field(getObjectClass(), getFieldID(), rhs); \
    } \
    template <> void JniField<TYPE, false>::set(const TYPE& rhs) { \
        mEnv->Set ## FUNCTION_NAME_FRAGMENT ## Field(mInstance, getFieldID(), rhs); \
    } \
    template <> void JniField<TYPE, true>::get(TYPE& result) const { \
        result = (TYPE) mEnv->GetStatic ## FUNCTION_NAME_FRAGMENT ## Field(getObjectClass(), getFieldID()); \
    } \
    template <> void JniField<TYPE, false>::get(TYPE& result) const { \
        result = (TYPE) mEnv->Get ## FUNCTION_NAME_FRAGMENT ## Field(mInstance, getFieldID()); \
    }

JniField_ACCESSORS(jstring, Object)
JniField_ACCESSORS(jobject, Object)
JniField_ACCESSORS(jboolean, Boolean)
JniField_ACCESSORS(jbyte, Byte)
JniField_ACCESSORS(jchar, Char)
JniField_ACCESSORS(jshort, Short)
JniField_ACCESSORS(jint, Int)
JniField_ACCESSORS(jlong, Long)
JniField_ACCESSORS(jfloat, Float)
JniField_ACCESSORS(jdouble, Double)

#undef JniField_ACCESSORS

*/

#endif /* JNIFIELD_H_ */