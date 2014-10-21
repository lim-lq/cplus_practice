/*
 * Author: blond.li 
 *
 * This filesystem use to test size fo send between Ice server and Ice client, and Ice thread
 *
 */

#pragma once

module Blond
{
    sequence<string> Files;

    interface FileInterface{
        idempotent string downloadfile(string file);
        idempotent Files getfiles();
        idempotent void uploadfile(string file);
    };
};