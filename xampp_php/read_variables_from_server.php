<?php 
// ??????? ???? ????? ?? ?????? ???? MySQL 
$servername = "localhost"; 
$username = "root"; 
$password = "";  
$dbname = "sql_personal_data";  
 
 
$conn = new mysqli($servername, $username, $password, $dbname); 
 
 
if ($conn->connect_error) { 
    die("Connection failed: " . $conn->connect_error); 
} 
 
 
$sql = "SELECT flag FROM control_send ORDER BY f_id DESC LIMIT 1";  
 
$result = $conn->query($sql); 
 
if ($result->num_rows > 0) { 
 
    $row = $result->fetch_assoc(); 
    $flg_value = $row["flag"]; 
 
    echo $flg_value; 
} else { 
    echo "0 results"; 
} 

print "xflag=";
echo $flg_value;
print "endf";
print " \n";

$conn->close(); 
?>