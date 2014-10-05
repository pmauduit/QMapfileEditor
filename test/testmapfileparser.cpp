#include "testmapfileparser.h"
#include "../mapfileparser.h"


/** tests construtor */
void TestMapfileParser::testInitMapfileParser()
{
  MapfileParser * p = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(p->isLoaded());
  QVERIFY(p->getMapName() == "World Map");
  delete p;

  // test with an invalid path
  p = new MapfileParser("/dev/urandom");
  QVERIFY(! p->isLoaded());
  delete p;
}

/** tests getters / setters for the map name */
void TestMapfileParser::testMapName() {
  MapfileParser * p = new MapfileParser();

  QVERIFY(p->getMapName() == "MS");

  p->setMapName("test setMapName()");
  QVERIFY(p->getMapName() == "test setMapName()");

  delete p;
}


/** tests getters / setters for the map filepath */
void TestMapfileParser::testFilePath() {
  MapfileParser * p = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(p->getMapfilePath().size() > 0);
  delete p;

  p = new MapfileParser();
  QVERIFY(p->getMapfilePath().isEmpty());
  delete p;

}

/** tests drawing map (MapfileParser::getCurrentMapImage() */
void TestMapfileParser::testGetCurrentMapImage() {
  MapfileParser * p = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(p->isLoaded());

  unsigned char * buffer = p->getCurrentMapImage(0,0);
  QVERIFY(buffer != NULL);

  QVERIFY(p->getCurrentMapImageSize() > 0);
  /** Should be PNG */
  QVERIFY(buffer[0] == 0x89 && buffer[1] == 0x50 && buffer[2] == 0x4e);

  // same with a given size
  buffer = p->getCurrentMapImage(500,500);
  QVERIFY(buffer != NULL);
  QVERIFY(p->getCurrentMapImageSize() > 0);
  QVERIFY(buffer[0] == 0x89 && buffer[1] == 0x50 && buffer[2] == 0x4e);
  QImage *im = new QImage();
  im->loadFromData(buffer, p->getCurrentMapImageSize());
  // Makes sure the generated PNG is 500x500
  QVERIFY(im->width() == 500 && im->height() == 500);

  if (im) delete im;
  if (p) delete p;

}

/** tests layers getters / setters */
void TestMapfileParser::testLayers() {
  MapfileParser * p = new MapfileParser("../data/world_mapfile.map");
  QVERIFY(p->isLoaded());

  QStringList layers = p->getLayers();
  QVERIFY(layers.size() == 2);
  QVERIFY(layers.at(0) == "world_raster");
  QVERIFY(layers.at(1) == "world_adm0");

  if (p) delete p;

  p = new MapfileParser();

  // on a blank mapfile, no layer
  QVERIFY(p->getLayers().size() == 0);

  if (p) delete p;
}

/** test map status */
void TestMapfileParser::testStatus() {
  MapfileParser * p  = new MapfileParser();

  // initial state: Map status is true
  QVERIFY(p->getMapStatus());

  p->setMapStatus(false);

  QVERIFY(! p->getMapStatus());

  if (p) delete p;

}
/** test map width / height */
void TestMapfileParser::testWidthHeight() {
  MapfileParser * p  = new MapfileParser();
  QVERIFY(p->getMapWidth() == -1);
  QVERIFY(p->getMapHeight() == -1);

  p->setMapSize(500,500);

  QVERIFY(p->getMapWidth() == 500);
  QVERIFY(p->getMapHeight() == 500);

  if (p) delete p;
}

/** test max size */
void TestMapfileParser::testMapMaxSize() {

  MapfileParser * p  = new MapfileParser();

  QVERIFY(p->getMapMaxsize() == 2048);

  p->setMapMaxsize(1024);

  QVERIFY(p->getMapMaxsize() == 1024);

  if (p) delete p;
}

