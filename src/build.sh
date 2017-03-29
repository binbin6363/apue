MAKEFILE_TIME=$(date)
USER_BUILD=$(whoami)
MACHINE_IP=$(/sbin/ifconfig | grep "inet"|grep -v "127.0.0.1" | grep -v "inet6" | awk '{print $2}'| tr "\n" " ")
GCC_VERSION=$(gcc --version | head -n 1 | sed 's/[()]//g')

git rev-list HEAD | sort > config.git-hash
LOCALVER=`wc -l config.git-hash | awk '{print $1}'`
if [ $LOCALVER \> 1 ] ; then
    VER=`git rev-list origin/master | sort | join config.git-hash - | wc -l | awk '{print $1}'`
    if [ $VER != $LOCALVER ] ; then
        VER="$VER+$(($LOCALVER-$VER))"
    fi
    if git status | grep -q "modified:" ; then
        VER="${VER}M"
    fi
    VER="$VER $(git rev-list HEAD -n 1 | cut -c 1-7)"
    GIT_VERSION=r$VER
else
    GIT_VERSION=
fi
rm -f config.git-hash


if [ ! "$GIT_VERSION" ]; then 
    GIT_VERSION=$(date +%s)
fi

MAKE_VERSION=1

if [ -f ".makev" ]; then 
    MAKE_VERSION=$(cat .makev) 
    MAKE_VERSION=$((MAKE_VERSION+1))
fi
echo $MAKE_VERSION > .makev

SERVER_VERSION="$GIT_VERSION.$MAKE_VERSION"

#OUT="ADD_DEFINITIONS(-DMAKEFILE_TIME=\"$MAKEFILE_TIME\" -DUSER_BUILD=\"$USER_BUILD\" -DMACHINE_IP=\"$MACHINE_IP\" -DGCC_VERSION=\"$GCC_VERSION\" -DSERVER_VERSION=\"$SERVER_VERSION\")" 

rm -f version.h
cp .version.h.template version.h
sed -i "s/\$BUILD_FULL_VERSION/\"$GIT_VERSION\"/g" version.h
sed -i "s/\$BUILD_USER/\"$USER_BUILD\"/g" version.h
sed -i "s/\$BUILD_MACHINE_IP/\"$MACHINE_IP\"/g" version.h
sed -i "s/\$BUILD_GCC_VERSION/\"$GCC_VERSION\"/g" version.h
sed -i "s/\$BUILD_MAKEFILE_TIME/\"$MAKEFILE_TIME\"/g" version.h


#echo $OUT > versioninfo.cmake
