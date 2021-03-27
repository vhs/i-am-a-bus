# PROTOCOL

These are the message we have found so far and what we assume they do.

## Legend
| Code | Description    |
| :--- | :------------- |
| C    | Controller     |
| S    | Sign           |
| B    | Bi-directional |
| s    | Sequence       |
| d    | Data           |

## Messages

| Direction | OpCode | Message                              | Designator                | Certainty |
| :-------- | :----- | :----------------------------------- | :------------------------ | :-------- |
| C         | :0000  | 0101                                 | DONE DATA ?               | Low       |
| S         | :0000  | 0401                                 | END OF STATUS DATA        | High      |
| C         | :0000  | 1001                                 | END OF LED DATA           | High      |
| S         | :0100  | 020400                               | OK                        | High      |
| S         | :0100  | 020401                               | LED DATA RECEIVED OKAY?   | Low       |
| S         | :0100  | 020407                               | SIGN WAITING FOR INPUT    | High      |
| S         | :0100  | 02040F                               | SIGN READY                | High      |
| S         | :0100  | 020591                               | READY RECEIVE LED DATA    | High      |
| S         | :0100  | 020595                               | READY RECEIVE SIGN CONFIG | High      |
| C         | :0100  | 0102FF                               | CONTROLLER READY          | High      |
| C         | :0100  | 020200                               | REQUEST STATUS            | High      |
| C         | :0100  | 0202FF                               | SIGN READY?               | Low       |
| C         | :0100  | 0203A1                               | REQUEST SEND SIGN CONFIG  | High      |
| C         | :0100  | 0203A2                               | REQUEST SEND LED DATA     | High      |
| C         | :0100  | 0203A8                               | SEND MORE STATUS          | High      |
| C         | :0100  | 0203AB                               | SEND STATUS               | High      |
| C         | :0100  | 020600                               | SET LED SIGN DATA         | High      |
| C         | :0100  | 0302FF                               | SIGN POWER ON STEP #1 ?   | Low       |
| C         | :0100  | 0402FF                               | SIGN POWER ON STEP #2 ?   | Low       |
| C         | :0100  | 0A02FF                               | SIGN POWER ON STEP #3 ?   | Low       |
| C         | :0100  | 0B02FF                               | SIGN POWER ON STEP #4 ?   | Low       |
| C         | :0100  | 7F02FF                               | CONTROLLER ALIVE?         | High      |
| B         | :1000  | ssssdddddddddddddddddddddddddddddddd | DATA                      | High      |
