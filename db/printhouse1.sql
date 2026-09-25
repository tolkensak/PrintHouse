-- MySQL Administrator dump 1.4
--
-- ------------------------------------------------------
-- Server version	5.1.42-community


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


--
-- Create schema printhouse1
--

CREATE DATABASE IF NOT EXISTS printhouse1;
USE printhouse1;

--
-- Definition of table `job`
--

DROP TABLE IF EXISTS `job`;
CREATE TABLE `job` (
  `tid` int(10) unsigned NOT NULL,
  `jid` int(10) unsigned NOT NULL,
  `size` varchar(255) DEFAULT NULL,
  `paper_fmt` varchar(255) DEFAULT NULL,
  `paper_num` int(10) unsigned DEFAULT NULL,
  `print_fmt` varchar(255) DEFAULT NULL,
  `color` varchar(255) DEFAULT NULL,
  `plast_num` int(10) unsigned DEFAULT NULL,
  `prior` double DEFAULT NULL,
  `sid` int(10) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`tid`,`jid`) USING BTREE,
  KEY `FK_job2state` (`sid`),
  CONSTRAINT `FK_job2state` FOREIGN KEY (`sid`) REFERENCES `state` (`sid`),
  CONSTRAINT `FK_job2task` FOREIGN KEY (`tid`) REFERENCES `task` (`tid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `job`
--

/*!40000 ALTER TABLE `job` DISABLE KEYS */;
INSERT INTO `job` (`tid`,`jid`,`size`,`paper_fmt`,`paper_num`,`print_fmt`,`color`,`plast_num`,`prior`,`sid`) VALUES 
 (9,1,'16х16','А4',230,'Ф23','Ц32',450,1,3),
 (10,1,'24х24','А5',5000,'Ф32','2+2',4000,2,2),
 (11,1,'56*56','А1',5000,'34*34','3+3',4000,0,2),
 (11,2,'567*567','457*457',75000,'347*347','37+37',74000,5,1),
 (11,3,'24х16','А2',5450,'gfgf','555',5450,7,4);
/*!40000 ALTER TABLE `job` ENABLE KEYS */;


--
-- Definition of table `jobh`
--

DROP TABLE IF EXISTS `jobh`;
CREATE TABLE `jobh` (
  `tid` int(10) unsigned NOT NULL,
  `jid` int(10) unsigned NOT NULL,
  `size` varchar(255) DEFAULT NULL,
  `paper_fmt` varchar(255) DEFAULT NULL,
  `paper_num` int(10) unsigned DEFAULT NULL,
  `print_fmt` varchar(255) DEFAULT NULL,
  `color` varchar(255) DEFAULT NULL,
  `plast_num` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`tid`,`jid`) USING BTREE,
  CONSTRAINT `FK_joba2taska` FOREIGN KEY (`tid`) REFERENCES `taskh` (`tid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `jobh`
--

/*!40000 ALTER TABLE `jobh` DISABLE KEYS */;
INSERT INTO `jobh` (`tid`,`jid`,`size`,`paper_fmt`,`paper_num`,`print_fmt`,`color`,`plast_num`) VALUES 
 (1,1,'45*45','78*8',50000,'89*89','6+6',60000);
/*!40000 ALTER TABLE `jobh` ENABLE KEYS */;


--
-- Definition of table `proc`
--

DROP TABLE IF EXISTS `proc`;
CREATE TABLE `proc` (
  `tid` int(10) unsigned NOT NULL,
  `jid` int(10) unsigned NOT NULL,
  `wid` int(10) unsigned NOT NULL,
  `vid` int(10) unsigned NOT NULL,
  `ord` int(10) unsigned NOT NULL,
  `uid` int(10) unsigned NOT NULL DEFAULT '0',
  `done` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`tid`,`jid`,`wid`,`vid`),
  KEY `FK_proc2value` (`wid`,`vid`),
  CONSTRAINT `FK_proc2job` FOREIGN KEY (`tid`, `jid`) REFERENCES `job` (`tid`, `jid`),
  CONSTRAINT `FK_proc2value` FOREIGN KEY (`wid`, `vid`) REFERENCES `value` (`wid`, `vid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `proc`
--

/*!40000 ALTER TABLE `proc` DISABLE KEYS */;
INSERT INTO `proc` (`tid`,`jid`,`wid`,`vid`,`ord`,`uid`,`done`) VALUES 
 (9,1,4,1,1,0,'0000-00-00 00:00:00'),
 (9,1,14,4,3,0,'0000-00-00 00:00:00'),
 (9,1,20,1,4,0,'0000-00-00 00:00:00'),
 (9,1,21,1,2,0,'0000-00-00 00:00:00'),
 (10,1,4,1,1,0,'0000-00-00 00:00:00'),
 (10,1,6,3,3,0,'0000-00-00 00:00:00'),
 (10,1,7,1,4,0,'0000-00-00 00:00:00'),
 (10,1,13,2,5,0,'0000-00-00 00:00:00'),
 (10,1,14,4,6,0,'0000-00-00 00:00:00'),
 (10,1,24,1,2,0,'0000-00-00 00:00:00'),
 (11,1,2,2,4,0,'0000-00-00 00:00:00'),
 (11,1,4,1,1,0,'0000-00-00 00:00:00'),
 (11,1,5,1,5,0,'0000-00-00 00:00:00'),
 (11,1,8,2,6,0,'0000-00-00 00:00:00'),
 (11,1,11,2,3,0,'0000-00-00 00:00:00'),
 (11,1,21,1,2,0,'0000-00-00 00:00:00'),
 (11,2,4,1,1,0,'0000-00-00 00:00:00'),
 (11,2,5,1,3,0,'0000-00-00 00:00:00'),
 (11,2,8,2,4,0,'0000-00-00 00:00:00'),
 (11,2,21,1,2,0,'0000-00-00 00:00:00'),
 (11,3,14,3,1,0,'0000-00-00 00:00:00');
/*!40000 ALTER TABLE `proc` ENABLE KEYS */;


--
-- Definition of table `proch`
--

DROP TABLE IF EXISTS `proch`;
CREATE TABLE `proch` (
  `tid` int(10) unsigned NOT NULL,
  `jid` int(10) unsigned NOT NULL,
  `wid` int(10) unsigned NOT NULL,
  `vid` int(10) unsigned NOT NULL,
  `ord` int(10) unsigned NOT NULL,
  `uid` int(10) unsigned NOT NULL DEFAULT '0',
  `done` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`tid`,`jid`,`wid`,`vid`),
  KEY `FK_proca2value` (`wid`,`vid`),
  CONSTRAINT `FK_proca2joba` FOREIGN KEY (`tid`, `jid`) REFERENCES `jobh` (`tid`, `jid`),
  CONSTRAINT `FK_proca2value` FOREIGN KEY (`wid`, `vid`) REFERENCES `value` (`wid`, `vid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `proch`
--

/*!40000 ALTER TABLE `proch` DISABLE KEYS */;
INSERT INTO `proch` (`tid`,`jid`,`wid`,`vid`,`ord`,`uid`,`done`) VALUES 
 (1,1,4,1,1,7,'2009-03-04 17:17:34'),
 (1,1,11,3,3,11,'2009-03-04 17:18:00'),
 (1,1,22,1,2,20,'2009-03-04 17:17:48');
/*!40000 ALTER TABLE `proch` ENABLE KEYS */;


--
-- Definition of table `role`
--

DROP TABLE IF EXISTS `role`;
CREATE TABLE `role` (
  `rid` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`rid`) USING BTREE,
  UNIQUE KEY `UQ_name` (`name`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

--
-- Dumping data for table `role`
--

/*!40000 ALTER TABLE `role` DISABLE KEYS */;
INSERT INTO `role` (`rid`,`name`) VALUES 
 (2,'Маркетолог'),
 (3,'Мастер'),
 (1,'Технолог');
/*!40000 ALTER TABLE `role` ENABLE KEYS */;


--
-- Definition of table `state`
--

DROP TABLE IF EXISTS `state`;
CREATE TABLE `state` (
  `sid` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`sid`) USING BTREE,
  UNIQUE KEY `UQ_name` (`name`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;

--
-- Dumping data for table `state`
--

/*!40000 ALTER TABLE `state` DISABLE KEYS */;
INSERT INTO `state` (`sid`,`name`) VALUES 
 (5,'Архив'),
 (4,'Готово'),
 (1,'Новый'),
 (2,'Старт'),
 (3,'Стоп');
/*!40000 ALTER TABLE `state` ENABLE KEYS */;


--
-- Definition of table `task`
--

DROP TABLE IF EXISTS `task`;
CREATE TABLE `task` (
  `tid` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `code` varchar(255) NOT NULL,
  `company` varchar(255) NOT NULL,
  `name` varchar(255) NOT NULL,
  `refer_date` date NOT NULL,
  `edition` int(10) unsigned NOT NULL DEFAULT '0',
  `contact` varchar(255) DEFAULT NULL,
  `note` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`tid`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8;

--
-- Dumping data for table `task`
--

/*!40000 ALTER TABLE `task` DISABLE KEYS */;
INSERT INTO `task` (`tid`,`code`,`company`,`name`,`refer_date`,`edition`,`contact`,`note`) VALUES 
 (9,'101/01','мединфо','журнал','2009-03-13',230,'Гульжан Аубакирова\r\n       продаже менеджер\r\n       701 255 12 34',''),
 (10,'101/02','microsoft казахстан','лист','2009-03-10',10000,'Игор Ториков\r\n      маркетинг менеджер\r\n      777 345 12 65\r\n      395 29 18',''),
 (11,'103/10','скиф трэйд','плакат','2009-03-11',5000,'Жасұлан Халықов\r\n         Маркетинг менеджер\r\n         707 398 45 46\r\n         256 67 95','');
/*!40000 ALTER TABLE `task` ENABLE KEYS */;


--
-- Definition of table `taskh`
--

DROP TABLE IF EXISTS `taskh`;
CREATE TABLE `taskh` (
  `tid` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `code` varchar(255) NOT NULL,
  `company` varchar(255) NOT NULL,
  `name` varchar(255) NOT NULL,
  `refer_date` date NOT NULL,
  `edition` int(10) unsigned NOT NULL DEFAULT '0',
  `contact` varchar(255) DEFAULT NULL,
  `note` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`tid`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

--
-- Dumping data for table `taskh`
--

/*!40000 ALTER TABLE `taskh` DISABLE KEYS */;
INSERT INTO `taskh` (`tid`,`code`,`company`,`name`,`refer_date`,`edition`,`contact`,`note`) VALUES 
 (1,'45/45','логи ком','реклама','2009-03-04',10000,NULL,'');
/*!40000 ALTER TABLE `taskh` ENABLE KEYS */;


--
-- Definition of table `user`
--

DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `uid` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `rid` int(10) unsigned NOT NULL,
  `wid` int(10) unsigned NOT NULL,
  `login` varchar(255) NOT NULL,
  `pass` varchar(255) NOT NULL,
  `fname` varchar(255) DEFAULT NULL,
  `mname` varchar(255) DEFAULT NULL,
  `lname` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`uid`) USING BTREE,
  UNIQUE KEY `UQ_login` (`login`) USING BTREE,
  KEY `FK_user2role` (`rid`),
  KEY `FK_user2work` (`wid`),
  CONSTRAINT `FK_user2role` FOREIGN KEY (`rid`) REFERENCES `role` (`rid`),
  CONSTRAINT `FK_user2work` FOREIGN KEY (`wid`) REFERENCES `work` (`wid`)
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=utf8;

--
-- Dumping data for table `user`
--

/*!40000 ALTER TABLE `user` DISABLE KEYS */;
INSERT INTO `user` (`uid`,`rid`,`wid`,`login`,`pass`,`fname`,`mname`,`lname`) VALUES 
 (3,2,1,'мар','1','Иван','','Иванов'),
 (4,3,6,'лам','1','Қасен','','Әсетов'),
 (5,3,5,'уф','1','Шалқар','','Аманов'),
 (6,3,3,'стр','1','Наташа','','Кликова'),
 (7,3,4,'скл','1','Зауре','','Қамбарова'),
 (8,3,2,'пре','1','Наташа','','Қарамолда'),
 (9,3,7,'кли','1','Айгуль','','Айгулова'),
 (10,3,8,'шта','1','Асем','','Асемова'),
 (11,3,11,'пер','1','Алекс','','Алексов'),
 (12,3,13,'выс','1','Жакен','Жакенович','Жакенов'),
 (13,3,14,'биг','1','Ерлан','','Ерланов'),
 (17,3,17,'кон','1','Сауле','','Саулина'),
 (18,1,1,'admin','01','Админ','','Админов'),
 (19,3,21,'ком','1','Серкик','','Серкиков'),
 (20,3,22,'мит','1','Света','','Сергина'),
 (21,3,23,'гто','1','Руслан','Русланұлы','Русланов'),
 (22,3,24,'сак','1','Жибек','','Ерменова');
/*!40000 ALTER TABLE `user` ENABLE KEYS */;


--
-- Definition of table `value`
--

DROP TABLE IF EXISTS `value`;
CREATE TABLE `value` (
  `wid` int(10) unsigned NOT NULL,
  `vid` int(10) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`wid`,`vid`) USING BTREE,
  UNIQUE KEY `UQ_name` (`wid`,`name`) USING BTREE,
  CONSTRAINT `FK_value2work` FOREIGN KEY (`wid`) REFERENCES `work` (`wid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `value`
--

/*!40000 ALTER TABLE `value` DISABLE KEYS */;
INSERT INTO `value` (`wid`,`vid`,`name`) VALUES 
 (2,1,'н/ф'),
 (2,2,'п'),
 (2,3,'стр'),
 (3,2,'г'),
 (3,1,'м'),
 (4,1,'пров'),
 (5,2,'г'),
 (5,1,'от'),
 (6,2,'гл л-о'),
 (6,1,'гл лицо'),
 (6,3,'мат л-о'),
 (6,4,'мат лицо'),
 (7,2,'г'),
 (7,1,'от'),
 (8,1,'гш'),
 (8,2,'ш'),
 (11,4,'вш'),
 (11,2,'пр'),
 (11,3,'ск'),
 (11,5,'тп'),
 (11,1,'тс'),
 (13,3,'сц'),
 (13,1,'т'),
 (13,2,'шм'),
 (14,1,'р'),
 (14,4,'сц'),
 (14,3,'т'),
 (14,2,'шм'),
 (17,1,'т'),
 (17,2,'шм'),
 (20,3,'р'),
 (20,4,'сц'),
 (20,1,'т'),
 (20,2,'шм'),
 (21,1,'печать'),
 (22,1,'печать'),
 (23,1,'печать'),
 (24,1,'печать');
/*!40000 ALTER TABLE `value` ENABLE KEYS */;


--
-- Definition of table `work`
--

DROP TABLE IF EXISTS `work`;
CREATE TABLE `work` (
  `wid` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`wid`) USING BTREE,
  UNIQUE KEY `UQ_name` (`name`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8;

--
-- Dumping data for table `work`
--

/*!40000 ALTER TABLE `work` DISABLE KEYS */;
INSERT INTO `work` (`wid`,`name`) VALUES 
 (21,'Kомори'),
 (14,'Биговка'),
 (1,'Все'),
 (13,'Высечка'),
 (23,'ГТО'),
 (7,'Клише'),
 (17,'Конгрев'),
 (6,'Лам'),
 (22,'Митсубиси'),
 (19,'Нумер'),
 (11,'Переплет'),
 (20,'Перфор'),
 (9,'Плени на УФ'),
 (2,'Препресс'),
 (15,'Резка'),
 (24,'Сакурай'),
 (4,'Склад'),
 (12,'Склейка'),
 (16,'Скотч'),
 (3,'СТР'),
 (18,'Тиснение'),
 (5,'УФ'),
 (10,'Фальц'),
 (8,'Штамп');
/*!40000 ALTER TABLE `work` ENABLE KEYS */;


--
-- Definition of procedure `sp_hist`
--

DROP PROCEDURE IF EXISTS `sp_hist`;

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`dba`@`%` PROCEDURE `sp_hist`(
	IN p_tid INT UNSIGNED)
BEGIN

	SELECT COUNT(*)
	INTO @omit
	FROM job
	WHERE tid=p_tid AND sid<>4;

	IF @omit=0 THEN
		START TRANSACTION;

		INSERT INTO taskh(code, company, name, refer_date, edition, contact, note)
		SELECT code, company, name, refer_date, edition, contact, note
		FROM task
		WHERE tid=p_tid;

		IF @@error_count<>0 THEN ROLLBACK;
		ELSE
			SET @tid=LAST_INSERT_ID();
			IF @tid=0 THEN ROLLBACK;
			ELSE
  			INSERT INTO jobh(tid, jid, size, paper_fmt, paper_num, print_fmt, color, plast_num)
	  		SELECT @tid, jid, size, paper_fmt, paper_num, print_fmt, color, plast_num
		  	FROM job
			  WHERE tid=p_tid;

  			IF @@error_count<>0 THEN ROLLBACK;
	  		ELSE
		  		INSERT INTO  proch(tid, jid, wid, vid, ord, uid, done)
			  	SELECT @tid, jid, wid, vid, ord, uid, done
				  FROM proc
  				WHERE tid=p_tid;

  				IF @@error_count<>0 THEN ROLLBACK;
	  			ELSE
		  		DELETE FROM proc WHERE tid=p_tid;

    				IF @@error_count<>0 THEN ROLLBACK;
	    			ELSE
		    			DELETE FROM job WHERE tid=p_tid;

    					IF @@error_count<>0 THEN ROLLBACK;
	    				ELSE
  	    				DELETE FROM task WHERE tid=p_tid;
	  	    			COMMIT;
				    	END IF;
  				  END IF;
  				END IF;
	  		END IF;
			END IF;
		END IF;

	END IF;

END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of procedure `sp_job`
--

DROP PROCEDURE IF EXISTS `sp_job`;

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`dba`@`%` PROCEDURE `sp_job`(
	IN p_rid INT UNSIGNED,
	IN p_wid INT UNSIGNED,
	IN p_DF VARCHAR(255))
BEGIN



	IF p_DF='' THEN
		SET p_DF='%Y.%m.%d';
	END IF;



	IF p_wid=1 THEN
		SELECT
      t.tid,
    	t.code,
		  j.jid,
  		DATE_FORMAT(t.refer_date, p_DF),
			t.company,
	  	t.name,
		  t.edition,
  		j.size,
			j.paper_fmt,
	  	j.paper_num,
		  j.print_fmt,
  		j.color,
      j.plast_num,
	  	t.note,
		  j.sid
  	FROM task AS t
			INNER JOIN job AS j ON j.tid=t.tid
	  ORDER BY j.prior;
	ELSE
		SELECT t.tid,
			t.code,
			j.jid,
			DATE_FORMAT(t.refer_date, p_DF),
			t.company,
			t.name,
			t.edition,
			j.size,
			j.paper_fmt,
			j.paper_num,
			j.print_fmt,
			j.color,
      j.plast_num,
			t.note,
			j.sid
		FROM task AS t
			INNER JOIN job AS j ON j.tid=t.tid
			INNER JOIN proc AS p ON p.tid=t.tid AND p.jid=j.jid
		WHERE (p_rid<>3 OR j.sid=2)
			AND p.wid=p_wid
			AND p.wid=(
				SELECT pp.wid
				FROM proc AS pp
				WHERE pp.tid=p.tid
					AND pp.jid=p.jid
					AND pp.uid=0
				ORDER BY pp.ord
				LIMIT 1)
		ORDER BY j.prior;
	END IF;



END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of procedure `sp_jobh`
--

DROP PROCEDURE IF EXISTS `sp_jobh`;

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`dba`@`%` PROCEDURE `sp_jobh`(
	IN p_DF VARCHAR(255))
BEGIN



	IF p_DF='' THEN
		SET p_DF='%Y.%m.%d';
	END IF;



		SELECT
			t.tid,
			t.code,
			j.jid,
			DATE_FORMAT(t.refer_date, p_DF),
			t.company,
			t.name,
			t.edition,
			j.size,
			j.paper_fmt,
			j.paper_num,
			j.print_fmt,
			j.color,
      j.plast_num,
			t.note
		FROM taskh AS t
			INNER JOIN jobh AS j ON j.tid=t.tid;



END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of procedure `sp_prior`
--

DROP PROCEDURE IF EXISTS `sp_prior`;

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`dba`@`%` PROCEDURE `sp_prior`(
	IN p_ahead INT,
  IN p_tid INT UNSIGNED,
	IN p_jid INT UNSIGNED,
	IN p_tidDest INT UNSIGNED,
	IN p_jidDest INT UNSIGNED)
BEGIN


	DECLARE p1 DOUBLE DEFAULT 0;
	DECLARE p2 DOUBLE DEFAULT 0;
	DECLARE priorDest DOUBLE;
	DECLARE has INT DEFAULT 1;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET has=0;


	SELECT j.prior
	INTO priorDest
	FROM job AS j
	WHERE j.tid=p_tidDest AND j.jid=p_jidDest;


	IF p_ahead THEN
	BEGIN
		DECLARE cur CURSOR FOR
		SELECT DISTINCT j.prior
		FROM job AS j
		WHERE j.prior<=priorDest
		ORDER BY j.prior DESC
		LIMIT 2;

		OPEN cur;
    FETCH cur INTO p1;
		IF has THEN
      FETCH cur INTO p2;
      IF NOT has THEN
        SET p2=p1-2;
  		END IF;
		ELSE
      SELECT MIN(DISTINCT prior) INTO p1 FROM job;
		  SET p2=p1-2;
		END IF;
		CLOSE cur;
	END;
	ELSE
	BEGIN
		DECLARE cur CURSOR FOR
		SELECT DISTINCT j.prior
		FROM job AS j
		WHERE j.prior>=priorDest
		ORDER BY j.prior
		LIMIT 2;

		OPEN cur;
    FETCH cur INTO p1;
		IF has THEN
      FETCH cur INTO p2;
      IF NOT has THEN
        SET p2=p1+2;
  		END IF;
		ELSE
      SELECT MAX(DISTINCT prior) INTO p1 FROM job;
		  SET p2=p1+2;
		END IF;
		CLOSE cur;
	END;
	END IF;


	UPDATE job AS j
	SET j.prior=(p1+p2)/2
	WHERE j.tid=p_tid AND j.jid=p_jid;


END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of procedure `sp_proc`
--

DROP PROCEDURE IF EXISTS `sp_proc`;

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`dba`@`%` PROCEDURE `sp_proc`(
  IN p_rid INT UNSIGNED,
  IN p_wid INT UNSIGNED,
  IN p_tid INT UNSIGNED,
  IN p_jid INT UNSIGNED)
BEGIN


  SELECT
    w.name,
    v.name,
    p.ord,
    p.uid
  FROM `work` AS w
    INNER JOIN `value` AS v ON v.wid=w.wid
    INNER JOIN proc AS p ON p.wid=w.wid AND p.vid=v.vid
  WHERE (p_rid<>3 OR p_wid=p.wid)
    AND p_tid=p.tid
    AND p_jid=p.jid
  ORDER BY p.ord;



END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of procedure `sp_proch`
--

DROP PROCEDURE IF EXISTS `sp_proch`;

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`dba`@`%` PROCEDURE `sp_proch`(
  IN p_tid INT UNSIGNED,
  IN p_jid INT UNSIGNED)
BEGIN


  SELECT
    w.name,
    v.name,
    p.ord,
    p.uid
  FROM `work` AS w
    INNER JOIN `value` AS v ON v.wid=w.wid
    INNER JOIN proch AS p ON p.wid=w.wid AND p.vid=v.vid
  WHERE p_tid=p.tid
    AND p_jid=p.jid
  ORDER BY p.ord;



END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of procedure `sp_state`
--

DROP PROCEDURE IF EXISTS `sp_state`;

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`dba`@`%` PROCEDURE `sp_state`(
  IN p_tid INT UNSIGNED,
  IN p_jid INT UNSIGNED)
BEGIN


  UPDATE job AS j
  SET j.sid=4
  WHERE j.tid=p_tid
    AND j.jid=p_jid
    AND NOT EXISTS (
      SELECT p.ord
      FROM proc AS p
      WHERE p.tid=p_tid
        AND p.jid=p_jid
        AND p.uid=0
    );



END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;



/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
