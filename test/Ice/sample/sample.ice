/*
 * This is a sample slice file
 * only use for test ice
 * author: blond.li
 */
#pragma once

module Filesystem{
    exception IOError {
        string reason;
    };
    
    interface Node {
        idempotent string name();
    };

    sequence<string> Lines;

    interface File extends Node {
        idempotent Lines read() throws IOError;
        idempotent void write(Lines test) throws IOError;
    };

    sequence<Node*> NodeSeq;
    interface Directory extends Node{
        idempotent NodeSeq list();
    };
};