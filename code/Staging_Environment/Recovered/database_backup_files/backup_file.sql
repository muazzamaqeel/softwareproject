-- MySQL dump 10.13  Distrib 8.0.35, for Win64 (x86_64)
--
-- Host: aws-dbtest.cjqugotdygrg.eu-central-1.rds.amazonaws.com    Database: scrummy
-- ------------------------------------------------------
-- Server version	8.0.33

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
SET @MYSQLDUMP_TEMP_LOG_BIN = @@SESSION.SQL_LOG_BIN;
SET @@SESSION.SQL_LOG_BIN= 0;

--
-- GTID state at the beginning of the backup 
--

SET @@GLOBAL.GTID_PURGED=/*!80000 '+'*/ '';

--
-- Table structure for table `Adminstrate`
--

DROP TABLE IF EXISTS `Adminstrate`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Adminstrate` (
  `idAdmin` int NOT NULL AUTO_INCREMENT,
  `FirstName` varchar(45) NOT NULL,
  `LastName` varchar(45) NOT NULL,
  `Username` varchar(255) NOT NULL,
  `Password` varchar(45) NOT NULL,
  `Email` varchar(45) NOT NULL,
  PRIMARY KEY (`idAdmin`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Adminstrate`
--

LOCK TABLES `Adminstrate` WRITE;
/*!40000 ALTER TABLE `Adminstrate` DISABLE KEYS */;
INSERT INTO `Adminstrate` VALUES (1,'Muazzam','Aqeel','muazzam','thu','hejdj@ndje.de');
/*!40000 ALTER TABLE `Adminstrate` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Permission`
--

DROP TABLE IF EXISTS `Permission`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Permission` (
  `idAPermission` int NOT NULL AUTO_INCREMENT,
  `PermissionName` varchar(255) NOT NULL,
  PRIMARY KEY (`idAPermission`)
) ENGINE=InnoDB AUTO_INCREMENT=29 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Permission`
--

LOCK TABLES `Permission` WRITE;
/*!40000 ALTER TABLE `Permission` DISABLE KEYS */;
INSERT INTO `Permission` VALUES (1,'CREATE_SPRINT'),(2,'UPDATE_SPRINT'),(3,'DELETE_SPRINT'),(4,'VIEW_SPRINT'),(5,'CREATE_SPRINT_BACKLOG'),(6,'UPDATE_SPRINT_BACKLOG'),(7,'DELETE_SPRINT_BACKLOG'),(8,'VIEW_SPRINT_BACKLOG'),(9,'CREATE_TASK'),(10,'UPDATE_TASK'),(11,'DELETE_TASK'),(12,'VIEW_TASK'),(13,'CREATE_SUBTASK'),(14,'UPDATE_SUBTASK'),(15,'DELETE_SUBTASK'),(16,'VIEW_SUBTASK'),(17,'CREATE_PRODUCT_BACKLOG'),(18,'UPDATE_PRODUCT_BACKLOG'),(19,'DELETE_PRODUCT_BACKLOG'),(20,'VIEW_PRODUCT_BACKLOG'),(21,'CREATE_USER_STORY'),(22,'UPDATE_USER_STORY'),(23,'DELETE_USER_STORY'),(24,'VIEW_USER_STORY'),(25,'CREATE_PROJECT'),(26,'UPDATE_PROJECT'),(27,'DELETE_PROJECT'),(28,'VIEW_PROJECT');
/*!40000 ALTER TABLE `Permission` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ProductBacklog`
--

DROP TABLE IF EXISTS `ProductBacklog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ProductBacklog` (
  `idProductBacklog` int NOT NULL AUTO_INCREMENT,
  `Name` varchar(45) NOT NULL,
  `Project_idProject` int NOT NULL,
  PRIMARY KEY (`idProductBacklog`,`Project_idProject`),
  KEY `fk_ProductBacklog_Project1_idx` (`Project_idProject`),
  CONSTRAINT `fk_ProductBacklog_Project1` FOREIGN KEY (`Project_idProject`) REFERENCES `Project` (`idProject`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ProductBacklog`
--

LOCK TABLES `ProductBacklog` WRITE;
/*!40000 ALTER TABLE `ProductBacklog` DISABLE KEYS */;
INSERT INTO `ProductBacklog` VALUES (1,'Test',1);
/*!40000 ALTER TABLE `ProductBacklog` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Project`
--

DROP TABLE IF EXISTS `Project`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Project` (
  `idProject` int NOT NULL AUTO_INCREMENT,
  `ProjectName` varchar(45) NOT NULL,
  `Description` varchar(255) DEFAULT NULL,
  `Adminstrate_idAdmin` int NOT NULL,
  PRIMARY KEY (`idProject`,`Adminstrate_idAdmin`),
  KEY `fk_Project_Adminstrate1_idx` (`Adminstrate_idAdmin`),
  CONSTRAINT `fk_Project_Adminstrate1` FOREIGN KEY (`Adminstrate_idAdmin`) REFERENCES `Adminstrate` (`idAdmin`)
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Project`
--

LOCK TABLES `Project` WRITE;
/*!40000 ALTER TABLE `Project` DISABLE KEYS */;
INSERT INTO `Project` VALUES (1,'Scrummy','Software Project',1),(5,'SoftwareProject','2023',1),(6,'SoftwareProject','2023',1),(7,'SoftwareProject','2023',1),(8,'SoftwareProject','2023',1),(9,'Ruth','kjnpokjpo',1),(10,'Scrummy','An amazing project',1);
/*!40000 ALTER TABLE `Project` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Role`
--

DROP TABLE IF EXISTS `Role`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Role` (
  `idRole` int NOT NULL AUTO_INCREMENT,
  `RoleName` varchar(45) NOT NULL,
  PRIMARY KEY (`idRole`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Role`
--

LOCK TABLES `Role` WRITE;
/*!40000 ALTER TABLE `Role` DISABLE KEYS */;
INSERT INTO `Role` VALUES (1,'Scrum Master'),(2,'Product Owner'),(3,'Team Member');
/*!40000 ALTER TABLE `Role` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Role_has_Permission`
--

DROP TABLE IF EXISTS `Role_has_Permission`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Role_has_Permission` (
  `Role_idRole` int NOT NULL,
  `Permission_idAPermission` int NOT NULL,
  PRIMARY KEY (`Role_idRole`,`Permission_idAPermission`),
  KEY `fk_Role_has_Permission_Permission1_idx` (`Permission_idAPermission`),
  KEY `fk_Role_has_Permission_Role1_idx` (`Role_idRole`),
  CONSTRAINT `fk_Role_has_Permission_Permission1` FOREIGN KEY (`Permission_idAPermission`) REFERENCES `Permission` (`idAPermission`),
  CONSTRAINT `fk_Role_has_Permission_Role1` FOREIGN KEY (`Role_idRole`) REFERENCES `Role` (`idRole`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Role_has_Permission`
--

LOCK TABLES `Role_has_Permission` WRITE;
/*!40000 ALTER TABLE `Role_has_Permission` DISABLE KEYS */;
INSERT INTO `Role_has_Permission` VALUES (1,1),(1,2),(1,3),(1,4),(1,5),(1,6),(1,7),(1,8),(1,9),(2,9),(1,10),(2,10),(1,11),(2,11),(1,12),(2,12),(1,13),(2,13),(1,14),(2,14),(1,15),(2,15),(1,16),(2,16),(2,17),(2,18),(2,19),(2,20),(2,21),(2,22),(2,23),(2,24),(1,26),(2,26),(1,28),(2,28);
/*!40000 ALTER TABLE `Role_has_Permission` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Sprint`
--

DROP TABLE IF EXISTS `Sprint`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Sprint` (
  `idSprint` int NOT NULL AUTO_INCREMENT,
  `StartDate` datetime NOT NULL,
  `EndDate` datetime NOT NULL,
  `Title` varchar(255) NOT NULL,
  `Project_idProject` int NOT NULL,
  PRIMARY KEY (`idSprint`,`Project_idProject`),
  KEY `fk_Sprint_Project1_idx` (`Project_idProject`),
  CONSTRAINT `fk_Sprint_Project1` FOREIGN KEY (`Project_idProject`) REFERENCES `Project` (`idProject`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Sprint`
--

LOCK TABLES `Sprint` WRITE;
/*!40000 ALTER TABLE `Sprint` DISABLE KEYS */;
INSERT INTO `Sprint` VALUES (1,'2023-11-21 14:51:34','2023-11-28 12:00:00','Start Up',1),(2,'0000-00-00 00:00:00','2024-01-01 00:00:00','Test',1),(3,'2023-11-28 17:39:37','2024-01-01 00:00:00','Test',1),(4,'2023-11-30 20:55:33','2024-01-01 00:00:00','Works',1);
/*!40000 ALTER TABLE `Sprint` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `SprintBacklog`
--

DROP TABLE IF EXISTS `SprintBacklog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `SprintBacklog` (
  `idSprintBacklog` int NOT NULL AUTO_INCREMENT,
  `Priority` varchar(45) NOT NULL,
  `Status` varchar(45) NOT NULL,
  `Sprint_idSprint` int NOT NULL,
  `Sprint_Project_idProject` int NOT NULL,
  PRIMARY KEY (`idSprintBacklog`,`Sprint_idSprint`,`Sprint_Project_idProject`),
  KEY `fk_SprintBacklog_Sprint1_idx` (`Sprint_idSprint`,`Sprint_Project_idProject`),
  CONSTRAINT `fk_SprintBacklog_Sprint1` FOREIGN KEY (`Sprint_idSprint`, `Sprint_Project_idProject`) REFERENCES `Sprint` (`idSprint`, `Project_idProject`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `SprintBacklog`
--

LOCK TABLES `SprintBacklog` WRITE;
/*!40000 ALTER TABLE `SprintBacklog` DISABLE KEYS */;
INSERT INTO `SprintBacklog` VALUES (1,'High','Done',1,1);
/*!40000 ALTER TABLE `SprintBacklog` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `SubtaskPB`
--

DROP TABLE IF EXISTS `SubtaskPB`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `SubtaskPB` (
  `idSubtaskPB` int NOT NULL AUTO_INCREMENT,
  `Title` varchar(45) NOT NULL,
  `Description` varchar(255) DEFAULT NULL,
  `Status` varchar(45) NOT NULL,
  `TaskPB_idTaskPB` int NOT NULL,
  `TaskPB_ProductBacklog_idProductBacklog` int NOT NULL,
  `TaskPB_ProductBacklog_Project_idProject` int NOT NULL,
  PRIMARY KEY (`idSubtaskPB`,`TaskPB_idTaskPB`,`TaskPB_ProductBacklog_idProductBacklog`,`TaskPB_ProductBacklog_Project_idProject`),
  KEY `fk_SubtaskPB_TaskPB1_idx` (`TaskPB_idTaskPB`,`TaskPB_ProductBacklog_idProductBacklog`,`TaskPB_ProductBacklog_Project_idProject`),
  CONSTRAINT `fk_SubtaskPB_TaskPB1` FOREIGN KEY (`TaskPB_idTaskPB`, `TaskPB_ProductBacklog_idProductBacklog`, `TaskPB_ProductBacklog_Project_idProject`) REFERENCES `TaskPB` (`idTaskPB`, `ProductBacklog_idProductBacklog`, `ProductBacklog_Project_idProject`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `SubtaskPB`
--

LOCK TABLES `SubtaskPB` WRITE;
/*!40000 ALTER TABLE `SubtaskPB` DISABLE KEYS */;
INSERT INTO `SubtaskPB` VALUES (5,'Subtask 1','we testing now','Done',4,1,1);
/*!40000 ALTER TABLE `SubtaskPB` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `SubtaskSB`
--

DROP TABLE IF EXISTS `SubtaskSB`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `SubtaskSB` (
  `idSubtask` int NOT NULL AUTO_INCREMENT,
  `Title` varchar(45) NOT NULL,
  `Description` varchar(255) DEFAULT NULL,
  `Status` varchar(45) NOT NULL,
  `Task_idTask` int NOT NULL,
  `Task_SprintBacklog_idSprintBacklog` int NOT NULL,
  `Task_SprintBacklog_Sprint_idSprint` int NOT NULL,
  `Task_SprintBacklog_Sprint_Project_idProject` int NOT NULL,
  PRIMARY KEY (`idSubtask`,`Task_idTask`,`Task_SprintBacklog_idSprintBacklog`,`Task_SprintBacklog_Sprint_idSprint`,`Task_SprintBacklog_Sprint_Project_idProject`),
  KEY `fk_Subtask_Task1_idx` (`Task_idTask`,`Task_SprintBacklog_idSprintBacklog`,`Task_SprintBacklog_Sprint_idSprint`,`Task_SprintBacklog_Sprint_Project_idProject`),
  CONSTRAINT `fk_Subtask_Task1` FOREIGN KEY (`Task_idTask`, `Task_SprintBacklog_idSprintBacklog`, `Task_SprintBacklog_Sprint_idSprint`, `Task_SprintBacklog_Sprint_Project_idProject`) REFERENCES `TaskSB` (`idTask`, `SprintBacklog_idSprintBacklog`, `SprintBacklog_Sprint_idSprint`, `SprintBacklog_Sprint_Project_idProject`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `SubtaskSB`
--

LOCK TABLES `SubtaskSB` WRITE;
/*!40000 ALTER TABLE `SubtaskSB` DISABLE KEYS */;
INSERT INTO `SubtaskSB` VALUES (1,'Subtask 1','we testing now','Done',1,1,1,1);
/*!40000 ALTER TABLE `SubtaskSB` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `TaskPB`
--

DROP TABLE IF EXISTS `TaskPB`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `TaskPB` (
  `idTaskPB` int NOT NULL AUTO_INCREMENT,
  `Title` varchar(45) NOT NULL,
  `Description` varchar(255) DEFAULT NULL,
  `Status` varchar(45) NOT NULL,
  `ProductBacklog_idProductBacklog` int NOT NULL,
  `ProductBacklog_Project_idProject` int NOT NULL,
  PRIMARY KEY (`idTaskPB`,`ProductBacklog_idProductBacklog`,`ProductBacklog_Project_idProject`),
  KEY `fk_TaskPB_ProductBacklog1` (`ProductBacklog_idProductBacklog`,`ProductBacklog_Project_idProject`),
  CONSTRAINT `fk_TaskPB_ProductBacklog1` FOREIGN KEY (`ProductBacklog_idProductBacklog`, `ProductBacklog_Project_idProject`) REFERENCES `ProductBacklog` (`idProductBacklog`, `Project_idProject`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `TaskPB`
--

LOCK TABLES `TaskPB` WRITE;
/*!40000 ALTER TABLE `TaskPB` DISABLE KEYS */;
INSERT INTO `TaskPB` VALUES (4,'Task 1','we testing now','Done',1,1);
/*!40000 ALTER TABLE `TaskPB` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `TaskSB`
--

DROP TABLE IF EXISTS `TaskSB`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `TaskSB` (
  `idTask` int NOT NULL AUTO_INCREMENT,
  `Title` varchar(45) NOT NULL,
  `Description` varchar(255) DEFAULT NULL,
  `Status` varchar(45) NOT NULL,
  `SprintBacklog_idSprintBacklog` int NOT NULL,
  `SprintBacklog_Sprint_idSprint` int NOT NULL,
  `SprintBacklog_Sprint_Project_idProject` int NOT NULL,
  PRIMARY KEY (`idTask`,`SprintBacklog_idSprintBacklog`,`SprintBacklog_Sprint_idSprint`,`SprintBacklog_Sprint_Project_idProject`),
  KEY `fk_Task_SprintBacklog1_idx` (`SprintBacklog_idSprintBacklog`,`SprintBacklog_Sprint_idSprint`,`SprintBacklog_Sprint_Project_idProject`),
  CONSTRAINT `fk_Task_SprintBacklog1` FOREIGN KEY (`SprintBacklog_idSprintBacklog`, `SprintBacklog_Sprint_idSprint`, `SprintBacklog_Sprint_Project_idProject`) REFERENCES `SprintBacklog` (`idSprintBacklog`, `Sprint_idSprint`, `Sprint_Project_idProject`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `TaskSB`
--

LOCK TABLES `TaskSB` WRITE;
/*!40000 ALTER TABLE `TaskSB` DISABLE KEYS */;
INSERT INTO `TaskSB` VALUES (1,'Task 1','we testing now','Done',1,1,1);
/*!40000 ALTER TABLE `TaskSB` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `User`
--

DROP TABLE IF EXISTS `User`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `User` (
  `idUser` int NOT NULL AUTO_INCREMENT,
  `FirstName` varchar(45) NOT NULL,
  `LastName` varchar(45) NOT NULL,
  `Username` varchar(255) NOT NULL,
  `password` varchar(64) DEFAULT NULL,
  `Email` varchar(45) NOT NULL,
  `Role_idRole` int DEFAULT NULL,
  PRIMARY KEY (`idUser`),
  KEY `fk_User_Role1_idx` (`Role_idRole`),
  CONSTRAINT `fk_User_Role1` FOREIGN KEY (`Role_idRole`) REFERENCES `Role` (`idRole`)
) ENGINE=InnoDB AUTO_INCREMENT=32 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `User`
--

LOCK TABLES `User` WRITE;
/*!40000 ALTER TABLE `User` DISABLE KEYS */;
INSERT INTO `User` VALUES (27,'Muazzam','Aqeel','mo','58d118590928d0b20a75aaaa4bd3a8cd361ff89b73e6d2087f09292be28816ad','muazzamaqeel14@gmail.com',1),(28,'Wesley','Barisch','wes','58d118590928d0b20a75aaaa4bd3a8cd361ff89b73e6d2087f09292be28816ad','wesley@outlook.com',2),(29,'keti','hazbiu','keti','0373c9c9204cc00c71d9ba36685849572c373cd8fc3c13405be57ea5bf1a9df9','test',3),(30,'N','I','NI','74d1052f85c343e729c8cd480092148f585e19cc5f7cfc08ef3e4340e45afb40','Weeeeeeeeeee',2),(31,'aida','aida','aida','a3f9a909aa816e10ace873b59ad22164424f63b1987f0624803739475c94c255','aida',3);
/*!40000 ALTER TABLE `User` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `UserStory`
--

DROP TABLE IF EXISTS `UserStory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `UserStory` (
  `idUserStory` int NOT NULL AUTO_INCREMENT,
  `Title` varchar(45) NOT NULL,
  `Description` varchar(45) DEFAULT NULL,
  `Status` varchar(45) NOT NULL,
  `ProductBacklog_idProductBacklog` int NOT NULL,
  `ProductBacklog_Project_idProject` int NOT NULL,
  PRIMARY KEY (`idUserStory`,`ProductBacklog_idProductBacklog`,`ProductBacklog_Project_idProject`),
  KEY `fk_UserStory_ProductBacklog1_idx` (`ProductBacklog_idProductBacklog`,`ProductBacklog_Project_idProject`),
  CONSTRAINT `fk_UserStory_ProductBacklog1` FOREIGN KEY (`ProductBacklog_idProductBacklog`, `ProductBacklog_Project_idProject`) REFERENCES `ProductBacklog` (`idProductBacklog`, `Project_idProject`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `UserStory`
--

LOCK TABLES `UserStory` WRITE;
/*!40000 ALTER TABLE `UserStory` DISABLE KEYS */;
INSERT INTO `UserStory` VALUES (1,'User Story 1','User Story Testing','In Progress',1,1);
/*!40000 ALTER TABLE `UserStory` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `User_has_Project`
--

DROP TABLE IF EXISTS `User_has_Project`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `User_has_Project` (
  `User_idUser` int NOT NULL,
  `Project_idProject` int NOT NULL,
  PRIMARY KEY (`User_idUser`,`Project_idProject`),
  KEY `fk_User_has_Project_Project1_idx` (`Project_idProject`),
  KEY `fk_User_has_Project_User_idx` (`User_idUser`),
  CONSTRAINT `fk_User_has_Project_Project1` FOREIGN KEY (`Project_idProject`) REFERENCES `Project` (`idProject`),
  CONSTRAINT `fk_User_has_Project_User` FOREIGN KEY (`User_idUser`) REFERENCES `User` (`idUser`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `User_has_Project`
--

LOCK TABLES `User_has_Project` WRITE;
/*!40000 ALTER TABLE `User_has_Project` DISABLE KEYS */;
/*!40000 ALTER TABLE `User_has_Project` ENABLE KEYS */;
UNLOCK TABLES;
SET @@SESSION.SQL_LOG_BIN = @MYSQLDUMP_TEMP_LOG_BIN;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2023-12-05 22:33:11
