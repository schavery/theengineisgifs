<?php
/**
 * @file common.inc
 *
 * includes function definitions for gifs
 * Steve Avery steve@theengineisred.com September 2012
 */


/* form for submitting images, used on admin.php */
$form = <<<EOT
	<form method="post" action="admin.php">
		<table>
			<tbody>
				<tr>
					<td><label for="url">Post a new .gif:</label></td>
				</tr>
				<tr class="control-group warning">
					<td><i class="icon-chevron-right icon-white"></i><input type="text" name="url" id="url" placeholder="URL" /></td>
				</tr>
				<tr>
					<td><label for="title">Title:</label></td>
				</tr>
				<tr class="control-group warning">
					<td><i class="icon-chevron-right icon-white"></i><input type="text" placeholder="The Engine is ..." name="title" id="title" /></td>
				</tr>
				<tr>
					<td class="centered"><input class="btn btn-inverse submission" type="submit" value="Queue" /></td>
				</tr>
			</tbody>
		</table>
	</form>
EOT;


function db_connect() {
	$db = mysql_connect('localhost:3306','gifmaster','Dt4qWpVcYEE6wB2y');
	mysql_select_db('gifs');
	return $db;
}


/*
 * receives post request from admin.php
 * if the post request has the proper keys (title and url)
 * then we clean the data and submit it to the database
 * also figures out about when a gif would be deployed
 */
function post($url, $title) {
	$db = db_connect();

	$url = mysql_real_escape_string(trim($url));
	$title = mysql_real_escape_string(trim($title));

	$sql = "INSERT INTO `history` (`submitted`,`title`,`url`) VALUES (UNIX_TIMESTAMP(),'" . $title . "','" . $url . "')";
	mysql_query($sql);

	// clean url once more for display on the page.
	$url = htmlentities($url);
	mysql_close();
}

function get_day() {
	$db = db_connect();
	// estimate display time
	$sql = "SELECT COUNT(`url`) FROM `history` WHERE `displayed` IS NULL AND `removed` != 1";
	$resource = mysql_query($sql);
	$days_ahead = mysql_result($resource,0,0);

	mysql_close();

	// counting solution to find future date not on a weekend
	$saturday = 6;
	$today = date('r');

	for($ii = 1; $ii <= $days_ahead; $ii++) {
		if(date('N',strtotime($today . ' +' . $ii . ' days')) >= $saturday) {
			$days_ahead++;
		}
	}
	$time = date('l \t\h\e jS',strtotime($today . ' +' . $days_ahead . ' days'));
	return $time;
}


/*
 * updates the markup in index.html
 * updates the db with the time that the current filename was run
 */
function set_next() {
	/*
	 * updates img tag in index.html
	 * sets runtime for that url
	 */
	$db = db_connect();
	
	$sql = 'SELECT `file` FROM `history` WHERE `displayed` IS NULL AND `removed` != 1 ORDER BY `submitted` ASC LIMIT 1';
	$resource = mysql_query($sql);
	if(mysql_num_rows($resource)) {
		$file = mysql_result($resource, 0, 0);
	} else {
		echo 'no results to show';
		mysql_close();
		return;
	}

	file_put_contents('../index.html',html($file));

	// set time.
	$sql = "UPDATE `history` SET `displayed` = UNIX_TIMESTAMP() WHERE `file` = '$file'";
	mysql_query($sql);

	mysql_close();
}


/*
 * finds urls to download and save, updates the db with filenames
 */
function dl_new_imgs() {
	ini_set('max_execution_time', 300);
	$db = db_connect();

	// first, find the highest filename, so we can name further files.
	$sql = 'SELECT `file` FROM `history` WHERE `file` IS NOT NULL ORDER BY ABS(`file`) DESC LIMIT 1';
	$resource = mysql_query($sql);
	if(mysql_num_rows($resource)) {
		$filename = mysql_result($resource, 0, 0);
		$filecount = substr($filename, 0, -4); // remove '.gif'
	} else {
		// there are either none to dl or none with names.
		$sql = 'SELECT COUNT(*) FROM `history` WHERE `file` IS NULL';
		$resource = mysql_query($sql);
		if(mysql_num_rows($resource)) {
			$filecount = 0;
		} else {
			mysql_close();
			echo 'none to download';
		}
	}

	// download any files that dont have a file name.
	$sql = "SELECT `url` FROM `history` WHERE `file` IS NULL";
	$resource = mysql_query($sql);
	$updates = array();
	while($row = mysql_fetch_assoc($resource)) {
		$filecount++;
		$filename = $filecount . '.gif';

		// just realized that theres a lot of potential insecurity here. boo.
		$ch = curl_init($row['url']);
		$fp = fopen("C:\\www\\vhosts\\theengineisgifs\\archive\\$filename", 'wb');
		curl_setopt($ch, CURLOPT_FILE, $fp);
		curl_setopt($ch, CURLOPT_HEADER, 0);
		curl_exec($ch);
		curl_close($ch);
		fclose($fp);

		$updates[$row['url']] = $filename;

		// get the frame for preview
		$image = new Imagick("C:\\www\\vhosts\\theengineisgifs\\archive\\$filename");
		$coalesced = $image->coalesceImages();

		foreach ($coalesced as $frame) {
			$framename = "C:\\www\\vhosts\\theengineisgifs\\archive\\frames\\$filename";
			$frame->writeImage($framename);
		break;
		}
	} // end while

	// create update statement for filenames.
	if(count($updates)) {
		foreach($updates as $url => $filename) {
			$sql = "UPDATE `history` SET `file` = '$filename' WHERE `url` = '$url';";
			mysql_query($sql);
		}
	}
	mysql_close();

	// TODO create jpg preview file for history view.
	//
}


/*
 * Generates markup for index.html at root of theengineis.gifs
 */
function html($filename) {
	$html = <<<EOD
<!DOCTYPE html>
<html>
	<head>
		<title>The Engine Is Gifs</title>
		<style>
			body {
				margin:0;
				padding:0;
				height:100%;
				width:100%;
				overflow:hidden;
			}
		</style>
	</head>
	<body>
EOD;
$html .= "\n			<img src='/archive/$filename' width='1280' height='1024' />\n";
$html .= <<<EOD
	</body>
</html>
EOD;

return $html;
}

function history() {
	// get previews of all gifs
	$db = db_connect();

	$sql = 'SELECT `submitted`,`displayed`,`file` FROM `history` WHERE `file` IS NOT NULL AND `removed` != 1 ORDER BY `submitted` ASC';
	$resource = mysql_query($sql);
	if(mysql_num_rows($resource)) {
		$rows = array();
		while($row = mysql_fetch_assoc($resource)) {
			$rows[] = $row;
		}
	} else {
		$history = '<tr><td>no results to display</tr></td>';
	}
	mysql_close();

	// TODO pagination
	if(!isset($history)) {
		$history = '';
		foreach($rows as $row) {
			$history .= '<tr>';
			$frame = '/archive/frames/' . $row['file'];
			$file = '/archive/' . $row['file'];
			$history .= '<td><a href="' . $file . '" target="_blank"><img class="tabled" src="' . $frame . '" /></a></td>';

			if($row['displayed']) { // format ze date
				$date = new DateTime();
				$date->setTimestamp($row['displayed']);
				$date->setTimezone(new DateTimeZone('America/Los_Angeles'));
				$date = $date->format('F j, Y, g:i a');
				$history .= '<td><span class="date">' . $date . '</span></td>';
			} else {
				$history .= '<td><span class="undisplayed">--</span></td>';
			}
			
			$history .= '<td><a class="btn btn-inverse show_this" href="javascript:void(0)">Display Now</a><a class="btn btn-danger delete_this" href="javascript:void(0)">Delete</a></td>';
			$history .= '</tr>' . "\n";
		}
	}
	
	return $history;
}

// marks a row as removed
function remove($filename) {
	$db = db_connect();
	$sql = 'UPDATE `history` SET `removed` = 1 WHERE `file` = \'' . $filename . '\'';
	mysql_query($sql);
	mysql_close();
}

function set_custom($filename) {
	$db = db_connect();
	$sql = "UPDATE `history` SET `displayed` = UNIX_TIMESTAMP() WHERE `file` = '$filename'";
	mysql_query($sql);
	file_put_contents('C:\\www\\vhosts\\theengineisgifs\\index.html',html($filename));
	mysql_close();
}

function history_table() {
	$history = history();
	$table = <<<EOL
		<div id="controls">
			<a href="javascript:void(0)" id="back" class="btn btn-inverse">Go Back</a>
			<a href="javascript:void(0)" id="cron" class="btn btn-inverse">Update Database</a>
		</div>
		<div id="history">
			<table class="table table-hover">
				<thead>
					<tr id="sticky">
						<th>Image</th>
						<th>Displayed</th>
						<th>Actions</th>
					</tr>
				</thead>
				<tbody>
					$history
				</tbody>
			</table>
		</div>
EOL;
	return $table;
}

/*
 * Elephant display is the new layout
 * thanks Tony for the name
 */
function elephant_display {
	// the output format.
	
}