-- Keep a log of any SQL queries you execute as you solve the mystery.

-- July 28, 2024 and that it took place on Humphrey Street.
SELECT * FROM crime_scene_reports
   ...> WHERE street='Humphrey Street';
-- 7/28/2024: id 295
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time
-- each of their interview transcripts mentions the bakery.

SELECT * FROM flights WHERE month=7 AND day=28 AND year=2024;
--Trying to find city but theres so many need to know the time

SELECT * FROM people LIMIT 10;
-- passport number and license plate might help

SELECT * from bakery_security_logs LIMIT 10;
-- security has each license plate

SELECT transcript FROM interviews WHERE day=28 AND month=7 AND year=2024;
-- Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
-- I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
-- As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket

-- look for atm transcations near that time
-- look for license plate
-- phone call made
-- flight is 7/29/24

SELECT * FROM phone_calls WHERE day=28 AND month=7 and year=2024;
--too many calls that day

SELECT license_plate FROM bakery_security_logs WHERE year=2024 AND month=7 AND day=28 AND hour=10;
--she said within 10 mins of theft, so between 10:15 and 10:25 of type exit
5P2BI95, 94KL13X, 6P58WS2, 4328GD8, G412CB7, L93JTIZ, 322W7JE, 0NTHK55

SELECT * FROM atm_transactions WHERE day=28 AND month=7 AND year=2024 AND transaction_type='withdraw' AND atm_location='Leggett Street';
--Really just tells me the account number
SELECT * FROM bank_accounts LIMIT 10;
--Can be cross verified with bank accounts and person_id

--We now are looking for overlap:
--People who: made a phone call under a minute, withdrew from ATM, exited the parking lot from 10:15-10:25,
--join phone_calls and people
--join atms with bank accounts with people and license plates from security log
--join flights wih passengers with people passports

SELECT * FROM people
   ...> WHERE phone_number
   ...> IN (SELECT caller FROM phone_calls
   ...> JOIN people ON phone_calls.caller = people.phone_number
   ...> WHERE day=28 AND month=7 AND year=2024 AND duration<60);
-- now compare license plates
SELECT license_plate FROM people
   WHERE phone_number
   IN (SELECT caller FROM phone_calls
   JOIN people ON phone_calls.caller = people.phone_number
   WHERE day=28 AND month=7 AND year=2024 AND duration<60)
   INTERSECT
   SELECT license_plate FROM bakery_security_logs
   WHERE day=28 AND month=7 AND year=2024 AND hour=10 AND minute>15 AND minute<25;
0NTHK55
322W7JE
94KL13X
G412CB7

--now look at atm withdrawals:
--go from atm_transactions w Humphrey Lane, withdrawal
--to bank accounts person_id
--to person license plate
SELECT license_plate FROM people
   JOIN bank_accounts ON people.id = bank_accounts.person_id
   WHERE bank_accounts.account_number
   IN
   (SELECT account_number FROM atm_transactions
   WHERE day=28 AND month=7 AND year=2024 AND transaction_type='withdraw' AND atm_location='Leggett Street');
'322W7JE', '94KL13X'
| 514354 | Diana | (770) 555-1861 | 3592750733      | 322W7JE
| 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X
--theif was diana or bruce
--now compare passport number with passengers who left on 7/29/24

SELECT * FROM airports LIMIT 10;
--airport: city = Fiftyville
--flight: origin_airport_id = 8

SELECT id FROM flights WHERE day=29 AND month=7 AND year=2024 AND origin_airport_id=8;
18, 23, 36, 43, 53
--now find passengers from there with passport # IN (3592750733, 5773159633)
SELECT passport_number FROM passengers
   WHERE flight_id IN
   (SELECT id FROM flights WHERE day=29 AND month=7 AND year=2024 AND origin_airport_id=8)
   AND passport_number IN (3592750733, 5773159633);
--this was no help they both took a flight
--they took the earliest flight
SELECT id FROM flights
   ...> WHERE day=29 AND month=7 AND year=2024 AND origin_airport_id=8
   ...> ORDER BY hour, minute ASC
   LIMIT 1;
id = 36

SELECT passport_number FROM passengers
   WHERE flight_id=36
   AND passport_number IN (3592750733, 5773159633);
5773159633
BRUCE

-- Destination is 4 or NYC

SELECT * FROM phone_calls
   WHERE caller='(367) 555-5533' AND day=28 AND month=7 AND year=2024 AND duration<60;
-- receiver = (375) 555-8161

--Robin
