#include "calculatelaylines_test.h"
#include <qtest.h>
#include <QSignalSpy>

#include "calculatelaylines.h"

QVector<QPointF> bm_calculateLaylines::longTermRoute;

void bm_calculateLaylines::test_data() {
    bm_calculateLaylines::longTermRoute << QPointF(25.1216, 65.0567)
                                        << QPointF(25.1219, 65.0569)
                                        << QPointF(25.2271, 65.0627)
                                        << QPointF(25.204, 65.0749)
                                        << QPointF(25.1577, 65.09)
                                        << QPointF(25.1153, 65.1039)
                                        << QPointF(25.105, 65.1101)
                                        << QPointF(25.1043, 65.1105)
                                        << QPointF(25.1038, 65.1109)
                                        << QPointF(25.1034, 65.1114)
                                        << QPointF(25.1031, 65.1118)
                                        << QPointF(25.1028, 65.1123)
                                        << QPointF(25.1027, 65.1128)
                                        << QPointF(25.1026, 65.1133)
                                        << QPointF(25.1027, 65.1137)
                                        << QPointF(25.1029, 65.1142)
                                        << QPointF(25.1031, 65.1147)
                                        << QPointF(25.1035, 65.1151)
                                        << QPointF(25.1039, 65.1155)
                                        << QPointF(25.1045, 65.116)
                                        << QPointF(25.1067, 65.1174)
                                        << QPointF(25.107, 65.1175)
                                        << QPointF(25.1073, 65.1177)
                                        << QPointF(25.108, 65.118)
                                        << QPointF(25.1088, 65.1184)
                                        << QPointF(25.1096, 65.1186)
                                        << QPointF(25.1105, 65.1189)
                                        << QPointF(25.1114, 65.1191)
                                        << QPointF(25.1123, 65.1193)
                                        << QPointF(25.1133, 65.1194)
                                        << QPointF(25.1143, 65.1196)
                                        << QPointF(25.1154, 65.1197)
                                        << QPointF(25.1164, 65.1197)
                                        << QPointF(25.1173, 65.1197)
                                        << QPointF(25.18, 65.1193)
                                        << QPointF(25.1815, 65.1195)
                                        << QPointF(25.1831, 65.1198)
                                        << QPointF(25.1844, 65.1201)
                                        << QPointF(25.1856, 65.1205)
                                        << QPointF(25.1868, 65.121)
                                        << QPointF(25.1877, 65.1214)
                                        << QPointF(25.1883, 65.1218)
                                        << QPointF(25.1892, 65.1224)
                                        << QPointF(25.1896, 65.123)
                                        << QPointF(25.1901, 65.1236)
                                        << QPointF(25.1904, 65.1243)
                                        << QPointF(25.1902, 65.125)
                                        << QPointF(25.19, 65.1257)
                                        << QPointF(25.1897, 65.1263)
                                        << QPointF(25.1889, 65.1271)
                                        << QPointF(25.1885, 65.1274)
                                        << QPointF(25.1881, 65.1277)
                                        << QPointF(25.1867, 65.1285)
                                        << QPointF(25.1832, 65.1305)
                                        << QPointF(25.1802, 65.1321)
                                        << QPointF(25.1716, 65.137)
                                        << QPointF(25.1558, 65.1459)
                                        << QPointF(25.1496, 65.1496)
                                        << QPointF(25.1107, 65.1833)
                                        << QPointF(25.1097, 65.184)
                                        << QPointF(25.1091, 65.1844)
                                        << QPointF(25.1086, 65.1847)
                                        << QPointF(25.1082, 65.185)
                                        << QPointF(25.1076, 65.1854)
                                        << QPointF(25.0794, 65.2015)
                                        << QPointF(25.0356, 65.2266)
                                        << QPointF(25.0143, 65.2388)
                                        << QPointF(25.0129, 65.2415);

    QTest::addColumn<QPointF>("boatPosition");

    QTest::newRow("1. boat position") << QPointF(25.087, 65.0584);
    QTest::newRow("2. boat position") << QPointF(25.216, 65.0702);
    QTest::newRow("3. boat position") << QPointF(25.104, 65.1128);
    QTest::newRow("4. boat position") << QPointF(25.1209, 65.1201);
    QTest::newRow("5. boat position") << QPointF(25.1893, 65.1278);
}

void bm_calculateLaylines::test() {
    QFETCH(QPointF, boatPosition);

    CalculateLaylines cl;
    cl.pathPoints = bm_calculateLaylines::longTermRoute;
    cl.startPoint = boatPosition;
    cl.calculationOnGoing = false;

    // To test that laylines are actually calculated
    // Should be in own unit test case
    //QSignalSpy emitLaylinesSpy(&cl, SIGNAL(emitLaylines(QVector<QPointF>)));

    QBENCHMARK {
        cl.calculationOnGoing = false;
        cl.startCalc();
    }

    //QVERIFY(emitLaylinesSpy.count() == 1);
}

QTEST_APPLESS_MAIN(bm_calculateLaylines)
