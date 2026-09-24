# Los metodos native de GameEngine no deben renombrarse ni eliminarse:
# sus nombres estan ligados a las firmas JNI en jni_bridge.cpp.
-keepclasseswithmembernames class com.hidanhell.juegorphg.GameEngine {
    native <methods>;
}
