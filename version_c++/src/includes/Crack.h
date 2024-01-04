#ifndef CRACK_NATIVE_H
#define CRACK_NATIVE_H

static jobject getContext(JNIEnv *env) {
    jclass activityThread = env->FindClass("android/app/ActivityThread");
    jmethodID currentActivityThread = env->GetStaticMethodID(activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
    jobject at = env->CallStaticObjectMethod(activityThread, currentActivityThread);
    jmethodID getApplication = env->GetMethodID(activityThread, "getApplication", "()Landroid/app/Application;");
    jobject context = env->CallObjectMethod(at, getApplication);
    return context;
}

static const char *LoaderAsserts(JNIEnv *env, const char *name) {

    jobject context = getContext(env);

    jclass clazzContext = env->FindClass("android/content/Context");
    jmethodID getAsserts = env->GetMethodID(clazzContext, "getAssets", "()Landroid/content/res/AssetManager;");
    jobject assetManager = env->CallObjectMethod(context, getAsserts);
    if (!assetManager) return NULL;

    jclass clazzAssetManager = env->FindClass("android/content/res/AssetManager");
    jmethodID methodOpen = env->GetMethodID(clazzAssetManager, "open", "(Ljava/lang/String;)Ljava/io/InputStream;");
    jobject inputStream = env->CallObjectMethod(assetManager, methodOpen, env->NewStringUTF(name));
    if (!inputStream) return NULL;

    jclass clazzInputStream = env->FindClass("java/io/InputStream");
    jmethodID methodAvailable = env->GetMethodID(clazzInputStream, "available", "()I");
    jint available = env->CallIntMethod(inputStream, methodAvailable);
    if (!available) return NULL;

    jbyteArray byteArray = env->NewByteArray(available);
    env->GetByteArrayElements(byteArray, NULL);

    jmethodID methodRead = env->GetMethodID(clazzInputStream, "read", "([B)I");
    jmethodID methodClose = env->GetMethodID(clazzInputStream, "close", "()V");
    env->CallVoidMethod(inputStream, methodRead, byteArray);
    env->CallVoidMethod(inputStream, methodClose);

    jclass clazzStandardCharsets = env->FindClass("java/nio/charset/StandardCharsets");
    jfieldID utf8Field = env->GetStaticFieldID(clazzStandardCharsets, "UTF_8", "Ljava/nio/charset/Charset;");
    jobject utf8Charset = env->GetStaticObjectField(clazzStandardCharsets, utf8Field);

    jclass clazzString = env->FindClass("java/lang/String");
    jmethodID methodString = env->GetMethodID(clazzString, "<init>", "([BLjava/nio/charset/Charset;)V");
    jobject javaString = env->NewObject(clazzString, methodString, byteArray, utf8Charset);

    jclass objectClass = env->FindClass("java/lang/Object");
    jmethodID toStringMethod = env->GetMethodID(objectClass, "toString", "()Ljava/lang/String;");
    jstring jcontent = (jstring) env->CallObjectMethod(javaString, toStringMethod);

    const char *content = env->GetStringUTFChars(jcontent, NULL);
    if (!content) return NULL;

    env->DeleteLocalRef(context);
    env->DeleteLocalRef(assetManager);
    env->DeleteLocalRef(inputStream);
    env->DeleteLocalRef(byteArray);
    env->DeleteLocalRef(javaString);
    env->DeleteLocalRef(utf8Charset);

    return content;
}

static jstring config(JNIEnv *env, jclass thiz, jint type, jboolean parseJson, jstring str) {
    try {

        const char *assertsConfig = LoaderAsserts(env, "config.json");
        if (!assertsConfig || strlen(assertsConfig) == 0) return env->NewStringUTF("");

        json config = json::parse(assertsConfig);
        const char *token = env->GetStringUTFChars(str, NULL);

        std::map<int, std::string> data = {
            {1, "app_config"},
            {2, "app_layout"},
            {3, "app_text"}
        };

        const std::string& update = data[type];
        string url = config[update];

        cpr::Header headers = cpr::Header{
            {"User-Agent", "DTunnelMod (@DTunnelMod, @DTunnelModGroup, @LightXVD)"},
            {"Dtunnel-Token", token},
            {"Dtunnel-Update", update},
        };

        cpr::Response req = cpr::Get(cpr::Url{url.c_str()}, headers);

        json content = json::parse(req.text.c_str());

        if (parseJson == JNI_TRUE) {
            json response = json::array();
            for (const auto& value : content) response.push_back(value.dump());
            return env->NewStringUTF(response.dump().c_str());
        }

        return env->NewStringUTF(content.dump().c_str());

    } catch (const std::exception& e) {
        return env->NewStringUTF("");
    }
}

#endif //CRACK_NATIVE_H
