#pragma once

module Blond
{
    interface ConfigureServerInterface{
        idempotent string getkey();
        idempotent int setkey(string key);
    };
};