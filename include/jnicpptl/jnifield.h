

#ifndef JNIFIELD_H_
#define JNIFIELD_H_

#include <jni.h>
#include <string>

class JniProxy;

template<typename T>
T JniGetStaticField(JNIEnv* env, jclass clazz, jfieldID field);

template<typename T>
void JniSetStaticField(JNIEnv* env, jclass clazz, jfieldID field, T value);

template<typename T>
T JniGetField(JNIEnv* env, jobject obj, jfieldID field);

template<typename T>
void JniSetField(JNIEnv* env, jobject obj, jfieldID field, T value);



template<typename T, bool isStatic>
class JniField_base;

template<typename T>
class JniField_base<T, false> {
protected:
	std::string mFieldName;
	std::string mFieldSignature;
	jfieldID mFieldID;

public:
	JniField_base(const char* name, const char* signature)
	: mFieldName(name)
	, mFieldSignature(signature)
	, mFieldID(NULL)
	{}

	jfieldID getFieldID(JNIEnv* env, jclass clazz);

	T get(JNIEnv* env, jobject obj);
	void set(JNIEnv* env, jobject obj, T value);
};

template<typename T>
class JniField_base<T, true>{

protected:
	std::string mFieldName;
	std::string mFieldSignature;
	jfieldID mFieldID;

public:
	JniField_base(const char* name, const char* signature)
	: mFieldName(name)
	, mFieldSignature(signature)
	, mFieldID(NULL)
	{}

	jfieldID getFieldID(JNIEnv* env, jclass clazz);

	T get(JNIEnv* env, jclass clazz);
	void set(JNIEnv* env, jclass obj, T value);
};

template<typename T, bool isStatic>
class JniField;

template<typename T>
class JniField<T, true> : JniField_base<T, true> {
	typedef JniField<T, true> SelfT;

protected:
	JniProxy* mProxy;

public:
	JniField(JniProxy* proxy, const char* name, const char* sig)
	: JniField_base<T, true>(name, sig)
	, mProxy(proxy)
	{}

	jfieldID getFieldID();

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

template<typename T>
class JniField<T, false> : JniField_base<T, false> {
	typedef JniField<T, false> SelfT;

protected:
	JniProxy* mProxy;

public:
	JniField(JniProxy* proxy, const char* name, const char* sig)
	: JniField_base<T, false>(name, sig)
	, mProxy(proxy)
	{}

	jfieldID getFieldID();

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
