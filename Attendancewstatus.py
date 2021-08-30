import serial
import pymysql.cursors
ser = serial.Serial('/dev/ttyACM0')
import face_recognition
import cv2
import numpy as np
from datetime import date
import time
video_capture = cv2.VideoCapture(0)
today = date.today().strftime("%Y-%m-%d")
ser.write(b'7')


connection = pymysql.connect(host='localhost',
                             user='dbuser',
                             password='renick',
                             database='attendance',
                             cursorclass=pymysql.cursors.DictCursor)

if not connection.open:
    print('opening conn')
    connection = pymysql.connect(host='localhost',
                             user='dbuser',
                             password='renick',
                             database='attendance',
                             cursorclass=pymysql.cursors.DictCursor)
with connection:
    with connection.cursor() as cursor:
        cursor.execute("DELETE FROM tbl_attendance WHERE attendance_date=%s;",str(today));
        cursor.execute("SELECT student_id,student_name FROM tbl_student;")
        ids=cursor.fetchall()
        for x in ids:
            #print(x)
            absent="INSERT INTO `tbl_attendance` (`student_id`, `attendance_status`, `attendance_date`,`teacher_id`) VALUES (%s,%s,%s,%s);";
            #print(absent)
            cursor.execute(absent, (x['student_id'], 'Absent', str(today),'6'));
        connection.commit()

# Load a sample picture and learn how to recognize it.
ren_image = face_recognition.load_image_file("renick.jpg")
ren_face_encoding = face_recognition.face_encodings(ren_image)[0]

# Load a second sample picture and learn how to recognize it.
rohan_image = face_recognition.load_image_file("rohan.jpg")
rohan_face_encoding = face_recognition.face_encodings(rohan_image)[0]

# Load a sample picture and learn how to recognize it.
santhosh_image = face_recognition.load_image_file("santhosh.jpg")
santhosh_face_encoding = face_recognition.face_encodings(santhosh_image)[0]
'''
# Load a second sample picture and learn how to recognize it.
gaurav_image = face_recognition.load_image_file("Gaurav.jpg")
gaurav_face_encoding = face_recognition.face_encodings(gaurav_image)[0]
'''
# Create arrays of known face encodings and their names
known_face_encodings = [
    ren_face_encoding,
    rohan_face_encoding,
    santhosh_face_encoding
]
known_face_names = [
    "Renick Andrews", "Rohan Sinha", "Santhosh Kumar"
]



# Initialize some variables
face_locations = []
face_encodings = []
face_names = []
marked=[]
process_this_frame = True
previous=''
previous_time=0
ser.write(b'3')
while True:    
    # Grab a single frame of video
    
    ret, frame = video_capture.read()
    

    # Resize frame of video to 1/4 size for faster face recognition processing
    small_frame = cv2.resize(frame, (0, 0), fx=0.33, fy=0.33)

    # Convert the image from BGR color (which OpenCV uses) to RGB color (which face_recognition uses)
    rgb_small_frame = small_frame[:, :, ::-1]
    
    # Only process every other frame of video to save time
    if process_this_frame:
        # Find all the faces and face encodings in the current frame of video
        face_locations = face_recognition.face_locations(rgb_small_frame)
        face_encodings = face_recognition.face_encodings(rgb_small_frame, face_locations)

        face_names = []
        for face_encoding in face_encodings:
            # See if the face is a match for the known face(s)
            matches = face_recognition.compare_faces(known_face_encodings, face_encoding)
            name = "Unknown"
            

            # # If a match was found in known_face_encodings, just use the first one.
            # if True in matches:
            #     first_match_index = matches.index(True)
            #     name = known_face_names[first_match_index]

            # Or instead, use the known face with the smallest distance to the new face
            face_distances = face_recognition.face_distance(known_face_encodings, face_encoding)
            best_match_index = np.argmin(face_distances)
            if matches[best_match_index]:
                name = known_face_names[best_match_index]
                
                if name =='Unknown':
                    print('Unknown')
                
                elif name != previous:
                    previous=name
                    previous_time=time.time()
                    if not name in marked:
                        print(name)                    
                        ser.write(b'2\n1')                       
                        cardid=ser.readline().decode('utf-8').strip()
                        #time.sleep(2)
                        print(cardid)
                        if not connection.open:
                            #print('opening conn')
                            connection = pymysql.connect(host='localhost',
                                 user='dbuser',
                                 password='renick',
                                 database='attendance',
                                 cursorclass=pymysql.cursors.DictCursor)
                            #absent="INSERT INTO `tbl_attendance` (`student_id`, `attendance_status`, `attendance_date`,`teacher_id`) VALUES (%s,%s,%s,%s)"
                            #cursor.execute(absent,
                        with connection:
                            with connection.cursor() as cursor:
                                sql="SELECT `student_id`, `card` FROM `tbl_student` WHERE `student_name`=%s"
                                cursor.execute(sql, (name))
                                result= cursor.fetchone()
                                #print(result)
                                #print(result['card'] == cardid)
                                if cardid == result['card']:                                    
                                    ser.write(b'\n4')
                                    sql="UPDATE `tbl_attendance` SET student_id= %s, attendance_status=%s, attendance_date=%s,teacher_id=%s WHERE student_id=%s;"
                                    cursor.execute(sql, (result['student_id'], 'Present', str(today),'6', result['student_id']))
                                    print("Updated attendance")
                                    time.sleep(2)
                                    connection.commit()                                    
                                    marked.append(name)
                                    
                                else:
                                    print("Invalid card")
                                    ser.write(b'6')
                                    time.sleep(2)
                    
                        
                    else:
                        print('Already marked!')
                        ser.write(b'5')
                        time.sleep(2)
                    
                
                else:
                    print("Please Wait...")
                    ser.write(b'3')
                    if int(time.time()) - int(previous_time) >= 5:
                        previous=''
                        previous_time=time.time()
                    

                    
               
            
            face_names.append(name)
            time.sleep(0.5)
            ser.write(b'3')
            time.sleep(1)
            

    process_this_frame = not process_this_frame


    # Display the results
    for (top, right, bottom, left), name in zip(face_locations, face_names):
        # Scale back up face locations since the frame we detected in was scaled to 1/4 size
        top *= 4
        right *= 4
        bottom *= 4
        left *= 4

        # Draw a box around the face
        cv2.rectangle(frame, (left, top), (right, bottom), (0, 0, 255), 2)

        # Draw a label with a name below the face
        cv2.rectangle(frame, (left, bottom - 35), (right, bottom), (0, 0, 255), cv2.FILLED)
        font = cv2.FONT_HERSHEY_DUPLEX
        cv2.putText(frame, name, (left + 6, bottom - 6), font, 1.0, (255, 255, 255), 1)

    # Display the resulting image
    cv2.imshow('Video', frame)

    # Hit 'q' on the keyboard to quit!
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release handle to the webcam
video_capture.release()
cv2.destroyAllWindows()


