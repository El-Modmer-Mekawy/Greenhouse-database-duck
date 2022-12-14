<?php
include('db.php');
$conn = connectionToSql();
if ($conn == mysqli_connect_error()) {
    echo "Connection database failed: " . mysqli_connect_error();
} else {
    $request_type = $_GET['request_type'];

    if ($request_type == "add") {
        $temperature = $_GET['temp'];
        $humidity = $_GET['hum'];
        $co2 = $_GET['co2'];
        $ch4 = $_GET['ch4'];
       

        //add data to the database
        $data = array();
        //insert data to the database
        $sql = "INSERT INTO SensorData (temp, hum, co2, ch4) VALUES ('$temp', '$hum', '$co2', '$ch4',)";
        if (mysqli_query($conn, $sql)) {
            $data = [
                "status" => "success",
                "reason" => "Data added successfully"
            ];
        } else {
            $data = [
                "status" => "failed",
                "reason" => "Error: " . $sql . "<br>" . mysqli_error($conn)
            ];
        }
        mysqli_close($conn);
        $data = json_encode($data);




    }elseif($request_type == "get"){
        
        $start_date = $_GET['start_date'];
        $end_date = $_GET['end_date'];
        //get date between two dates
        if (isset($start_date) && isset($end_date)) {
            if ($start_date == "0" && $end_date == "0") {
                $sql = "SELECT * FROM SensorData";
            } else if ($start_date == "0") {
                $sql = "SELECT * FROM SensorData WHERE date <= '$end_date'";

            } else if ($end_date == "0") {
                $sql = "SELECT * FROM SensorData WHERE date >= '$start_date'";
            } else {
                $sql = "SELECT * FROM SensorData WHERE date BETWEEN '$start_date' AND '$end_date'";
            }
        }else{
            $sql = "SELECT * FROM SensorData";
        }
        $result = mysqli_query($conn, $sql);
        if (mysqli_num_rows($result) > 0) {
            // output data in json format
            $data = array();
            $data['status'] = "success";
            $data['data'] = array();
            while ($row = mysqli_fetch_assoc($result)) {
                $data['data'][] = $row;
            }
        } else {
            $data['status'] = "failed";
            $data['reason'] = "No data found";
        }
        mysqli_close($conn);
        $data = json_encode($data);



    }else{
        $data['status'] = "failed";
        $data['reason'] = "You have to specify the request type, either add or get";
        $data = json_encode($data);
    }
    echo $data;
}

?>