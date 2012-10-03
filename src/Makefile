#==stuff linked to
DYNAMIC = -lSDL_mixer -lSDL_ttf -lSDL_image -lSDL -lm
#==global Flags. Even on the gp2x with 16 kb Cache, -O3 is much better then -Os
CFLAGS = -O3 -fsingle-precision-constant -fPIC 
CFLAGS_ASM = -O2 -fsingle-precision-constant -fPIC
GENERAL_TWEAKS = -ffast-math -fgcse-sm -fsched-spec-load -fmodulo-sched -fgcse-las -ftracer -funsafe-loop-optimizations -Wunsafe-loop-optimizations -fvariable-expansion-in-unroller
SMALL_RESOLUTION_DEVICES = -DFAST_MULTIPLICATION -DFAST_DIVISION
#==PC==
CPP = g++ -g -march=native -DX86CPU $(GENERAL_TWEAKS)
SDL = `sdl-config --cflags` -DSDL_INCLUDE_SUBDIR -I../../sparrow3d
#==Consoles==
#==GP2X/WIZ==
ifeq ($(TARGET),open2x)
CPP = /opt/open2x/g++-4.1.1-glibc-2.3.6/bin/arm-open2x-linux-g++ -DMOBILE_DEVICE -DARMCPU -DGP2X -DF100 $(GENERAL_TWEAKS) $(SMALL_RESOLUTION_DEVICES)
SDL = `/opt/open2x/g++-4.1.1-glibc-2.3.6/bin/sdl-config --cflags`
INCLUDE = -I/opt/open2x/g++-4.1.1-glibc-2.3.6/include
LIB = -L/opt/open2x/g++-4.1.1-glibc-2.3.6/lib -Wl,-rpath=/opt/open2x/g++-4.1.1-glibc-2.3.6/lib
endif
ifeq ($(TARGET),gp2x)
CPP = /opt/open2x/g++-4.1.1-glibc-2.3.6/bin/arm-open2x-linux-g++ -DMOBILE_DEVICE -DARMCPU -DGP2X -DF100 $(GENERAL_TWEAKS) $(SMALL_RESOLUTION_DEVICES)
STATIC = -Wl,-Bstatic -lSDL -lm -Wl,-Bdynamic
DYNAMIC = -lSDL_image -lSDL_ttf -lfreetype -lpng -lz -ljpeg
SDL = `/opt/open2x/g++-4.1.1-glibc-2.3.6/bin/sdl-config --cflags`
INCLUDE = -I/opt/open2x/g++-4.1.1-glibc-2.3.6/include
LIB = -L/opt/open2x/g++-4.1.1-glibc-2.3.6/lib -Wl,-rpath=/opt/open2x/g++-4.1.1-glibc-2.3.6/lib
endif
ifeq ($(TARGET),wiz)
CPP = /opt/open2x/g++-4.1.1-glibc-2.3.6/bin/arm-open2x-linux-g++ -DMOBILE_DEVICE -DARMCPU -DGP2X -DWIZ $(GENERAL_TWEAKS) $(SMALL_RESOLUTION_DEVICES)
SDL = `/opt/open2x/g++-4.1.1-glibc-2.3.6/bin/sdl-config --cflags`
INCLUDE = -I/opt/open2x/g++-4.1.1-glibc-2.3.6/include
LIB = -L/opt/open2x/g++-4.1.1-glibc-2.3.6/lib -Wl,-rpath=/opt/open2x/g++-4.1.1-glibc-2.3.6/lib
endif
#==Caanoo==
ifeq ($(TARGET),caanoo)
CPP = /opt/caanoo/g++-4.2.4-glibc-2.7-eabi/bin/arm-gph-linux-gnueabi-g++ -DMOBILE_DEVICE -DARMCPU -DCAANOO $(GENERAL_TWEAKS) $(SMALL_RESOLUTION_DEVICES)
SDL = -I/opt/caanoo/g++-4.2.4-glibc-2.7-eabi/arm-gph-linux-gnueabi/sys-root/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
INCLUDE = -I/opt/caanoo/g++-4.2.4-glibc-2.7-eabi/arm-gph-linux-gnueabi/sys-root/usr/include
LIB = -L/opt/caanoo/g++-4.2.4-glibc-2.7-eabi/arm-gph-linux-gnueabi/sys-root/usr/lib -Wl,-rpath=/opt/caanoo/g++-4.2.4-glibc-2.7-eabi/arm-gph-linux-gnueabi/sys-root/usr/lib
endif
#==Dingux==
ifeq ($(TARGET),dingux)
CPP = /opt/opendingux-toolchain/usr/bin/mipsel-linux-g++ -DMOBILE_DEVICE -DDINGUX $(GENERAL_TWEAKS) $(SMALL_RESOLUTION_DEVICES)
SDL = -I/opt/opendingux-toolchain/usr/mipsel-unknown-linux-uclibc/sys-include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
INCLUDE = -I/opt/opendingux-toolchain/usr/mipsel-unknown-linux-uclibc/sys-include
LIB = -L/opt/opendingux-toolchain/usr/lib -Wl,-rpath=/opt/opendingux-toolchain/usr/lib
endif
#==Pandora==
ifeq ($(TARGET),pandora)
#CPP = /opt/pandora/bin/arm-angstrom-linux-gnueabi-g++ -DARMCPU -DPANDORA $(GENERAL_TWEAKS)
#SDL = `/opt/pandora/bin/sdl-config --cflags`
#INCLUDE = -I/opt/pandora/arm-angstrom-linux-gnueabi/usr/include -I/opt/pandora/arm-angstrom-linux-gnueabi/usr/include/SDL
#LIB = -L/opt/pandora/arm-angstrom-linux-gnueabi/usr/lib -Wl,-rpath=/opt/pandora/arm-angstrom-linux-gnueabi/usr/lib
CPP = /opt/pandora/arm-2011.03/bin/arm-none-linux-gnueabi-g++ -DMOBILE_DEVICE -DARMCPU -DPANDORA $(GENERAL_TWEAKS)
SDL = `/opt/pandora/arm-2011.03/usr/bin/sdl-config --cflags`
INCLUDE = -I/opt/pandora/arm-2011.03/usr/include
LIB = -L/opt/pandora/arm-2011.03/usr/lib -Wl,-rpath=/opt/pandora/arm-2011.03/usr/lib
endif
#==Maemo 5==
ifeq ($(TARGET),maemo5)
CPP = g++ -DARMCPU -DMAEMO -DMAEMO5 $(GENERAL_TWEAKS)
SDL = `sdl-config --cflags`
endif
#==Maemo 6==
ifeq ($(TARGET),maemo6)
CPP = g++ -DARMCPU -DMAEMO -DMAEMO6 $(GENERAL_TWEAKS)
SDL = `sdl-config --cflags`
endif


all: Project4

targets:
	@echo "gp2x, open2x (like gp2x, but dynamic compiled => smaller), wiz caanoo, dingux, pandora, maemo5, maemo6"

Project4: main.cpp Application.o ProjectileLaser.o ShapeCircle.o ShapeRay.o ShapeRect.o SimpleDirection.o StateBase.o StateCollision.o StateLevel.o Timer.o UnitBase.o UnitBomb.o UnitLaser.o UnitPlayer.o UnitSpike.o HighscoreFile.o Replay.o StateHighscores.o StateMenu.o StateReplayLoader.o StateScore.o SpawnNormal.o ScoreNormal.o SpawnBase.o ScoreBase.o ItemSlowmo.o ItemVortex.o SpawnWaves.o StateError.o UtilityFunctions.o
	$(CPP) $(CFLAGS) main.cpp Application.o ProjectileLaser.o ShapeCircle.o ShapeRay.o ShapeRect.o SimpleDirection.o StateBase.o StateCollision.o StateLevel.o Timer.o UnitBase.o UnitBomb.o UnitLaser.o UnitPlayer.o UnitSpike.o HighscoreFile.o Replay.o StateHighscores.o StateMenu.o StateReplayLoader.o StateScore.o SpawnNormal.o ScoreNormal.o SpawnBase.o ScoreBase.o ItemSlowmo.o ItemVortex.o SpawnWaves.o StateError.o UtilityFunctions.o $(SDL) $(INCLUDE) -L../../sparrow3d $(LIB) $(STATIC) $(DYNAMIC) -lsparrow3d -o ../build/Project4


Application.o: Application.cpp Application.h
	$(CPP) $(CFLAGS) -c Application.cpp $(SDL) $(INCLUDE)

HighscoreFile.o: HighscoreFile.cpp HighscoreFile.h
	$(CPP) $(CFLAGS) -c HighscoreFile.cpp $(SDL) $(INCLUDE)

ItemSlowmo.o: ItemSlowmo.cpp ItemSlowmo.h
	$(CPP) $(CFLAGS) -c ItemSlowmo.cpp $(SDL) $(INCLUDE)

ItemVortex.o: ItemVortex.cpp ItemVortex.h
	$(CPP) $(CFLAGS) -c ItemVortex.cpp $(SDL) $(INCLUDE)

ProjectileLaser.o: ProjectileLaser.cpp ProjectileLaser.h
	$(CPP) $(CFLAGS) -c ProjectileLaser.cpp $(SDL) $(INCLUDE)

#Random.o: Random.cpp Random.h
#	$(CPP) $(CFLAGS) -c Random.cpp $(SDL) $(INCLUDE)

Replay.o: Replay.cpp Replay.h
	$(CPP) $(CFLAGS) -c Replay.cpp $(SDL) $(INCLUDE)

ScoreNormal.o: ScoreNormal.cpp ScoreNormal.h
	$(CPP) $(CFLAGS) -c ScoreNormal.cpp $(SDL) $(INCLUDE)

ScoreBase.o: ScoreBase.cpp ScoreBase.h
	$(CPP) $(CFLAGS) -c ScoreBase.cpp $(SDL) $(INCLUDE)

ShapeCircle.o: ShapeCircle.cpp ShapeCircle.h
	$(CPP) $(CFLAGS) -c ShapeCircle.cpp $(SDL) $(INCLUDE)

ShapeRay.o: ShapeRay.cpp ShapeRay.h
	$(CPP) $(CFLAGS) -c ShapeRay.cpp $(SDL) $(INCLUDE)

ShapeRect.o: ShapeRect.cpp ShapeRect.h
	$(CPP) $(CFLAGS) -c ShapeRect.cpp $(SDL) $(INCLUDE)

SimpleDirection.o: SimpleDirection.cpp SimpleDirection.h
	$(CPP) $(CFLAGS) -c SimpleDirection.cpp $(SDL) $(INCLUDE)

SpawnNormal.o: SpawnNormal.cpp SpawnNormal.h
	$(CPP) $(CFLAGS) -c SpawnNormal.cpp $(SDL) $(INCLUDE)

SpawnBase.o: SpawnBase.cpp SpawnBase.h
	$(CPP) $(CFLAGS) -c SpawnBase.cpp $(SDL) $(INCLUDE)

SpawnWaves.o: SpawnWaves.cpp SpawnWaves.h
	$(CPP) $(CFLAGS) -c SpawnWaves.cpp $(SDL) $(INCLUDE)

StateBase.o: StateBase.cpp StateBase.h
	$(CPP) $(CFLAGS) -c StateBase.cpp $(SDL) $(INCLUDE)

StateCollision.o: StateCollision.cpp StateCollision.h
	$(CPP) $(CFLAGS) -c StateCollision.cpp $(SDL) $(INCLUDE)

StateError.o: StateError.cpp StateError.h
	$(CPP) $(CFLAGS) -c StateError.cpp $(SDL) $(INCLUDE)

StateHighscores.o: StateHighscores.cpp StateHighscores.h
	$(CPP) $(CFLAGS) -c StateHighscores.cpp $(SDL) $(INCLUDE)

StateLevel.o: StateLevel.cpp StateLevel.h
	$(CPP) $(CFLAGS) -c StateLevel.cpp $(SDL) $(INCLUDE)

StateMenu.o: StateMenu.cpp StateMenu.h
	$(CPP) $(CFLAGS) -c StateMenu.cpp $(SDL) $(INCLUDE)

StateReplayLoader.o: StateReplayLoader.cpp StateReplayLoader.h
	$(CPP) $(CFLAGS) -c StateReplayLoader.cpp $(SDL) $(INCLUDE)

StateScore.o: StateScore.cpp StateScore.h
	$(CPP) $(CFLAGS) -c StateScore.cpp $(SDL) $(INCLUDE)

Timer.o: Timer.cpp Timer.h
	$(CPP) $(CFLAGS) -c Timer.cpp $(SDL) $(INCLUDE)

UnitBase.o: UnitBase.cpp UnitBase.h
	$(CPP) $(CFLAGS) -c UnitBase.cpp $(SDL) $(INCLUDE)

UnitBomb.o: UnitBase.cpp UnitBomb.h
	$(CPP) $(CFLAGS) -c UnitBomb.cpp $(SDL) $(INCLUDE)

UnitLaser.o: UnitLaser.cpp UnitLaser.h
	$(CPP) $(CFLAGS) -c UnitLaser.cpp $(SDL) $(INCLUDE)

UnitPlayer.o: UnitPlayer.cpp UnitPlayer.h
	$(CPP) $(CFLAGS) -c UnitPlayer.cpp $(SDL) $(INCLUDE)

UnitSpike.o: UnitSpike.cpp UnitSpike.h
	$(CPP) $(CFLAGS) -c UnitSpike.cpp $(SDL) $(INCLUDE)
	
UtilityFunctions.o: UtilityFunctions.cpp UtilityFunctions.h
	$(CPP) $(CFLAGS) -c UtilityFunctions.cpp $(SDL) $(INCLUDE)

clean:
	rm *.o
	rm ../build/Project4
