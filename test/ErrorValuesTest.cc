/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ErrorValues.hh"
#include "Word.hh"
#include <cassert>

void testError() {
    int smallError = 1;
    Letter l1 = 16;
    Letter l2 = 17;
    ErrorValues ev;
    ErrorValues ev2;

    assert(ev.getSubstituteError(l1, l2) == ErrorValues::getDefaultError());
    ev.setError(l1, l2, smallError);
    assert(ev.getSubstituteError(l1, l2) == smallError);
    assert(ev.getSubstituteError(l2, l1) == smallError);

    assert(ev2.getSubstituteError(l2, l1) == ErrorValues::getDefaultError());
    ev2.setError(l2, l1, smallError);
    assert(ev2.getSubstituteError(l1, l2) == smallError);
    assert(ev2.getSubstituteError(l2, l1) == smallError);
}

void testGroupError() {
    ErrorValues ev;
    Letter e = 101;
    Letter eacute = 0xe9;
    Letter ebreve = 0x115;

    assert(ev.getSubstituteError(e, eacute) == ErrorValues::getDefaultError());
    ev.addDefaultAccents();
    assert(ev.getSubstituteError(e, eacute) == ErrorValues::getDefaultGroupError());
    assert(ev.getSubstituteError(eacute, e) == ErrorValues::getDefaultGroupError());
    assert(ev.getSubstituteError(eacute, ebreve) == ErrorValues::getDefaultGroupError());
    assert(ev.getSubstituteError(e, ebreve) == ErrorValues::getDefaultGroupError());
}

int main(int argc, char **argv) {
    testError();
    testGroupError();
    return 0;
}

