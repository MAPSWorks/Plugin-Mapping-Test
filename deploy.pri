
#QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
unix {
  QMAKE_RPATHDIR += $ORIGIN
  QMAKE_RPATHDIR += $ORIGIN/Qt/lib

  isEmpty(PREFIX) {
    PREFIX = /usr
  }

  PREFIX = $$INSTALL_ROOT/$$PREFIX
  INSTALLBASE_LIB = $$PREFIX/lib/uavos
  INSTALLBASE_RES = $$PREFIX/share/uavos

}

!hpux {
  QMAKE_COPY_FILE = $${QMAKE_COPY_FILE} -P -p timestamps
}

# platform dependent install
android {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/src/android

    DISTFILES += \
        AndroidManifest.xml \
        ../android/gradle/wrapper/gradle-wrapper.jar \
        ../android/gradlew \
        ../android/res/values/libs.xml \
        ../android/build.gradle \
        ../android/gradle/wrapper/gradle-wrapper.properties \
        ../android/gradlew.bat

    uavosdata.path = /assets
    uavosdata.files += ../../data
    INSTALLS += uavosdata

    x86 {
        target.path = /libs/x86
    } else: armeabi-v7a {
        target.path = /libs/armeabi-v7a
    } else {
        target.path = /libs/armeabi
    }
    export(target.path)
    INSTALLS += target

}else:unix {
  target.path = $$INSTALLBASE_LIB
  INSTALLS += target

  targetx.path = $$INSTALLBASE_LIB
  targetx.files = ../shared/qt.conf
  targetx.extra = mkdir -p $$PREFIX/bin && ln -sf -t $$PREFIX/bin/ ../lib/uavos/$$TARGET
  INSTALLS += targetx

  # Qt libraries
  qtlibs.extra = ../../copy-libs.sh $$DESTDIR/$$TARGET $$INSTALLBASE_LIB/Qt/lib
  qtlibs.path = $$INSTALLBASE_LIB/Qt/lib


  uavos-data {
    INSTALLS -= target targetx
    INSTALLS += qtlibs
  }
}

export(INSTALLS)

