/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * Authors:
 *    Jussi Pakkanen <jussi.pakkanen@canonical.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of version 3 of the GNU Lesser General Public License as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <boost/python.hpp>
#include "columbus.hh"

using namespace boost::python;
using namespace Columbus;


void (Document::*addAdaptor) (const Word &, const WordList &) = &Document::addText;
void (Matcher::*queryAdaptor) (const WordList &, MatchResults &) = &Matcher::match;

BOOST_PYTHON_MODULE(_columbus)
{
    class_<Corpus, boost::noncopyable>("Corpus", init<>())
        .def("size", &Corpus::size)
        .def("add_document", &Corpus::addDocument)
        .def("__len__", &Corpus::size)
    ;

    class_<Word>("Word", init<const std::string&>())
            .def("get_string", &Word::asUtf8)
            .def("__len__", &Word::length)
            ;

    class_<WordList>("WordList", init<>())
            .def(init<const WordList &>())
            .def("__len__", &WordList::size)
            .def("__getitem__", &WordList::operator[],
                    return_internal_reference<1, with_custodian_and_ward<1, 2> >())
            .def("add_word", &WordList::addWord)
            ;

    def("_split_to_words", splitToWords);

    class_<Document>("Document", init<DocumentID>())
            .def(init<const Document&>())
            .def("field_count", &Document::fieldCount)
            .def("get_id", &Document::getID)
            .def("get_text", &Document::getText,
                    return_internal_reference<1, with_custodian_and_ward<1, 2> >())
            .def("add_text", addAdaptor)
            ;

    class_<MatchResults>("MatchResults")
            .def("add_results", &MatchResults::addResults)
            .def("get_document_id", &MatchResults::getDocumentID)
            .def("get_relevancy", &MatchResults::getRelevancy)
            .def("__len__", &MatchResults::size)
            ;

    class_<Matcher>("Matcher")
            .def("index", &Matcher::index)
            .def("match", queryAdaptor)
            .def("get_errorvalues", &Matcher::getErrorValues,
                    return_internal_reference<1, with_custodian_and_ward<1, 2> >())
             .def("get_indexweigths", &Matcher::getIndexWeights,
                    return_internal_reference<1, with_custodian_and_ward<1, 2> >())
            ;

    class_<ErrorValues>("ErrorValues")
            .def("add_standard_errors", &ErrorValues::addStandardErrors)
            .def("set_substring_mode", &ErrorValues::setSubstringMode)
            .def("set_error", &ErrorValues::setError)
            .def("get_substitute_error", &ErrorValues::getSubstituteError)
            .def("get_default_error", &ErrorValues::getDefaultError)
            .staticmethod("get_default_error")
            .def("clear_errors", &ErrorValues::clearErrors)
            ;

    class_<IndexWeights>("IndexWeights")
            .def("set_weight", &IndexWeights::setWeight)
            .def("get_weight", &IndexWeights::getWeight)
            ;
}
