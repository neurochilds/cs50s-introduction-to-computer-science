-- Keep a log of any SQL queries you execute as you solve the mystery.
.schema -- Identify database structure

/* Find crime scene report regarding the stolen CS50 duck  */
SELECT *
FROM crime_scene_reports
WHERE description LIKE '%duck%';

/* Crime scene report said three witnesses mentioned 'bakery' in their transcripts. Locate these transcripts with the following. */
SELECT *
FROM interviews
WHERE transcript LIKE '%bakery%';

/* The above query returned one transcript that was not about the theft. The modified query below eliminates that. */
SELECT *
FROM interviews
WHERE transcript LIKE '%bakery%' AND transcript LIKE '%thief%';

/* One interview transcript said that the thief drove away within 10 minutes of the theft. The below query searches the security logs for possible number plates */
SELECT *
FROM bakery_security_logs
WHERE hour = 10 AND minute > 14 AND minute < 26;

/* Tweaking above query after seeing data in the table. This is more specific to the crime window and only return suspect license plates.  */
SELECT license_plate
FROM bakery_security_logs
WHERE month = 7 AND day = 28 AND hour = 10 AND minute > 14 AND minute < 26 AND activity = 'exit';

/* Get information on all people whose license plates implicates them as suspects based on above query. */
SELECT *
FROM people
WHERE license_plate IN
    (SELECT license_plate
    FROM bakery_security_logs
    WHERE month = 7 AND day = 28 AND hour = 10 AND minute > 14 AND minute < 26 AND activity = 'exit');

/* One interview transcript stated that the thief called someone for less than 1 minute. This query returns the shortest phone calls on that day. */
SELECT *
FROM phone_calls
WHERE day = 28
ORDER BY duration
LIMIT 10;

/* The shortest call returned from above query was >30 mins, suggesting the thief kept the call going for longer than 1 minute.
The query below finds the details of all people who made a call on the day of the crime AND who license plate places them at the scene of the crime. */
SELECT *
FROM people
WHERE license_plate IN
    (SELECT license_plate
    FROM bakery_security_logs
    WHERE month = 7 AND day = 28 AND hour = 10 AND minute > 14 AND minute < 26 AND activity = 'exit')
AND phone_number IN
    (SELECT caller
    FROM phone_calls
    WHERE day = 28);

/* One interview transcript stated that the thief called someone to book the first flight out of Fiftyville tomorrow. This query returns the airport id for fiftyville. */
SELECT id
FROM airports
WHERE city = 'Fiftyville';

/* And this query returns the first flight out of Fiftyville on the day after the theft. */
SELECT *
FROM flights
WHERE origin_airport_id =
    (SELECT id
    FROM airports
    WHERE city = 'Fiftyville')
AND day = 29
ORDER BY hour, minute
LIMIT 1;

/* This returns the *destination city* of the thief's flight out of Fiftyville. */
SELECT city
FROM airports
WHERE id =
    (SELECT destination_airport_id
    FROM flights
    WHERE origin_airport_id =
        (SELECT id
        FROM airports
        WHERE city = 'Fiftyville')
    AND day = 29
    ORDER BY hour,minute
    LIMIT 1);

/* This returns the details of all passengers on the thiefs flight. */
SELECT *
FROM passengers
WHERE flight_id =
    (SELECT id
    FROM flights
    WHERE origin_airport_id =
        (SELECT id
        FROM airports
        WHERE city = 'Fiftyville')
    AND day = 29
    ORDER BY hour, minute
    LIMIT 1);

/* One interview transcript stated that the thief was seen withdrawing money from Leggett Street earlier on the day of the robbery. This query returns possible suspects. */
SELECT person_id
FROM bank_accounts
WHERE account_number IN
    (SELECT account_number
    FROM atm_transactions
    WHERE month = 7 AND day = 28 AND transaction_type = 'withdraw' AND atm_location = 'Leggett Street');

/* This returns the details of all passengers on the thiefs flight, whose license plate, phone call and bank withdrawal activity implicates them at the scene of the crime.
It returns one name, *Bruce*, who must be the thief*/
SELECT *
FROM people
WHERE passport_number IN
    (SELECT passport_number
    FROM passengers
    WHERE flight_id =
        (SELECT id
        FROM flights
        WHERE origin_airport_id =
            (SELECT id
            FROM airports
            WHERE city = 'Fiftyville')
        AND day = 29
        ORDER BY hour, minute
        LIMIT 1))
AND license_plate IN
    (SELECT license_plate
    FROM bakery_security_logs
    WHERE month = 7 AND day = 28 AND hour = 10 AND minute > 14 AND minute < 26 AND activity = 'exit')
AND phone_number IN
    (SELECT caller
    FROM phone_calls
    WHERE day = 28)
AND id IN
    (SELECT person_id
    FROM bank_accounts
    WHERE account_number IN
        (SELECT account_number
        FROM atm_transactions
        WHERE month = 7 AND day = 28 AND transaction_type = 'withdraw' AND atm_location = 'Leggett Street'));

/* This returns names of all people Bruce called during the day of the crime.*/
SELECT name
FROM people
WHERE phone_number IN
    (SELECT receiver
    FROM phone_calls
    WHERE day = 28
    AND caller =
        (SELECT phone_number
        FROM people
        WHERE name = 'Bruce'));

/* This returns the names of all people on the same flight as Bruce.*/
SELECT name
FROM people
WHERE passport_number IN
    (SELECT passport_number
    FROM passengers
    WHERE flight_id =
        (SELECT id
        FROM flights
        WHERE origin_airport_id =
            (SELECT id
            FROM airports
            WHERE city = 'Fiftyville')
        AND day = 29
        ORDER BY hour, minute
        LIMIT 1));

/* This returns the names of all people on the same flight as Bruce who he also called on the day of the crime.
It returns nothing, suggesting the accomplice did not board the flight with Bruce.*/
SELECT name
FROM people
WHERE phone_number IN
    (SELECT receiver
    FROM phone_calls
    WHERE day = 28
    AND caller =
        (SELECT phone_number
        FROM people
        WHERE name = 'Bruce'))
AND passport_number IN
    (SELECT passport_number
    FROM passengers
    WHERE flight_id =
        (SELECT id
        FROM flights
        WHERE origin_airport_id =
            (SELECT id
            FROM airports
            WHERE city = 'Fiftyville')
        AND day = 29
        ORDER BY hour, minute
        LIMIT 1));

/* The following query returns all atm transactions of people Bruce called on the night of the crime. It only returns the details of one account number.
This suggests the other three people he called do not have bank accounts and so could not have bought the plane ticket. It suggests the owner of this account is his accomplice.*/
SELECT *
FROM atm_transactions
WHERE account_number IN
    (SELECT account_number
    FROM bank_accounts
    WHERE person_id IN
        (SELECT id FROM people
        WHERE phone_number IN
            (SELECT receiver
            FROM phone_calls
            WHERE day = 28
            AND caller =
                (SELECT phone_number
                FROM people
                WHERE name = 'Bruce'))));

/* This query returns the name of the only person who Bob rang on the day of the crime who also has a bank account. This person must be his accomplice. It's *Robin*. */
SELECT name
FROM people
WHERE id =
    (SELECT person_id
    FROM bank_accounts
    WHERE account_number =
        (SELECT account_number
        FROM atm_transactions
        WHERE account_number IN
            (SELECT account_number
            FROM bank_accounts
            WHERE person_id IN
                (SELECT id FROM people
                WHERE phone_number IN
                    (SELECT receiver
                    FROM phone_calls
                    WHERE day = 28
                    AND caller =
                        (SELECT phone_number
                        FROM people
                        WHERE name = 'Bruce'))))));