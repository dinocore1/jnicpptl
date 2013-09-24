
#ifndef JNIOBJECT_H_
#define JNIOBJECT_H_

#include <jni.h>
#include <string>

class JniString;
class JniObject;

#include <jnimethod.h>


class JniObject {

private:
	JNIEnv* mEnv;
	string mClassname;
	jobject mInstance;

	jclass mCachedClassObj;

public:
	

	JniObject(JNIEnv* env, const char* name)
	: mEnv(env)
	, mClassname(name)
	{}

	JniObject(JNIEnv* env, jobject instance)
	: mEnv(env)
	, mInstance(instance)
	{
		JniMethod<jstring()> getName("getName", "()Ljava/lang/String;");
		JniString className(env, getName());
		mClassname = &className;
	}

	jclass getClass() {
		if(mCachedClassObj != NULL){
			mCachedClassObj = mEnv->FindClass(mClassname);
		}
		return mCachedClassObj;
	}
};

class JniString {
private:
	JNIEnv* env;
	jstring mString;
	bool isValid;
	const jchar* mCachedCharBuf;

public:
	JniString(JNIEnv* env, jstring str)
	: mEnv(env)
	, mString(str)
	, isValid(false)
	{}

	~JniString()
	{
		if(isValid){
			mEnv->ReleaseStringChars(mString, mCachedCharBuf);
			isValid = false;
		}
	}

	const char* operator& (){
		if(!isValid) {
			mCachedCharBuf = mEnv->GetStringChars(mString, NULL);
			isValid = true;
		}
		return mCachedCharBuf;
	}
};



#endif /* JNIOBJECT_H_ */