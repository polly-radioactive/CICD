#!/bin/bash

if [[ $CI_JOB_STATUS == 'success' ]]; then
    CI_JOB_STATUS="SUCCESS"
else
    CI_JOB_STATUS="FAILED"
fi

TG_TOKEN="6462809454:AAEYdxXQUUD0Ta5MqRwqmxtywm3c6UcJJII"
CHAT_ID="-1001980601791"
TEXT="Job name: $CI_JOB_NAME Status: $CI_JOB_STATUS Project: $CI_PROJECT_NAME"
curl --data chat_id=$CHAT_ID --data-urlencode "text=$TEXT" "https://api.telegram.org/bot$TG_TOKEN/sendMessage?parse_mode=HTML"
