

#ifndef JNIFIELD_H_
#define JNIFIELD_H_

#include <jni.h>
#include <cstdio>
#include <jniexception.h>


/**
 * Hides the details of accessing Java fields via JNI.
 */
template <typename NativeT, bool isStatic>
class JniField {
    typedef JniField<NativeT, isStatic> Self;
    
    const char* m_fieldName;
    const char* m_fieldSignature;
    JNIEnv* m_env;
    jobject m_instance;
    
    
public:

	JniField(const char* name, const char* signature)
    : m_fieldName(name)
    , m_fieldSignature(signature)
    {}

    
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
        return m_instance;
    }
    
private:
    void get(NativeT&) const;
    void set(const NativeT&);
    
    jfieldID getFieldID() const {
        // It's not a valid optimization to cache field ids in face of class unloading.
        // We could keep a global reference to the class to prevent it being unloaded, but that seems unfriendly.
        jfieldID result = isStatic ? m_env->GetStaticFieldID(getObjectClass(), m_fieldName, m_fieldSignature) 
        	: m_env->GetFieldID(getObjectClass(), m_fieldName, m_fieldSignature);
        if (result == 0) {
				m_env->ExceptionClear();
                JniException::throwErrorNow(m_env, "unable to find %s. (%s)", m_fieldName, m_fieldSignature);
				
        }
        return result;
    }
    
    jclass getObjectClass() const {
        // The JNI specification (http://java.sun.com/j2se/1.5.0/docs/guide/jni/spec/functions.html) suggests that GetObjectClass can't fail, so we don't need to check for exceptions.
        return m_env->GetObjectClass(m_instance);
    }
};

#define JniField_ACCESSORS(TYPE, FUNCTION_NAME_FRAGMENT) \
    template <> void JniField<TYPE, true>::set(const TYPE& rhs) { \
        m_env->SetStatic ## FUNCTION_NAME_FRAGMENT ## Field(getObjectClass(), getFieldID(), rhs); \
    } \
    template <> void JniField<TYPE, false>::set(const TYPE& rhs) { \
        m_env->Set ## FUNCTION_NAME_FRAGMENT ## Field(m_instance, getFieldID(), rhs); \
    } \
    template <> void JniField<TYPE, true>::get(TYPE& result) const { \
        result = (TYPE) m_env->GetStatic ## FUNCTION_NAME_FRAGMENT ## Field(getObjectClass(), getFieldID()); \
    } \
    template <> void JniField<TYPE, false>::get(TYPE& result) const { \
        result = (TYPE) m_env->Get ## FUNCTION_NAME_FRAGMENT ## Field(m_instance, getFieldID()); \
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



#endif /* JNIFIELD_H_ */