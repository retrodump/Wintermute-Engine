<?

  $target_path = "minidumps/";
  $target_path = $target_path . basename( $_FILES['crashfile']['name']);

  if(GetExtension($target_path) != "zip")
  {
    echo "Invalid file.";
    return;
  }

  if($_FILES['crashfile']['size'] > 1024*1024)
  {
    echo "The file is too large.";
    return;
  }

  if(move_uploaded_file($_FILES['crashfile']['tmp_name'], $target_path))
  {
    chmod($target_path, 0666);
    echo "The file " . basename($_FILES['crashfile']['name']) . " has been uploaded";

    $logline = basename($_FILES['crashfile']['name']) . " | " . gethostbyaddr($_SERVER['REMOTE_ADDR']) . "\r\n";
    error_log($logline, 3, 'minidumps/minidumps.log.php');
  }
  else
  {
    echo "There was an error uploading the file, please try again!";
  }


  function GetExtension($filename) 
  { 
    $filename = strtolower($filename) ; 
    $exts = split("[/\\.]", $filename) ; 
    $n = count($exts)-1;
    if($n <= 0) return "";
    return $exts[$n]; 
  } 

?>
