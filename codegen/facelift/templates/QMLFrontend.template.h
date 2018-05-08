{#*********************************************************************
**
** Copyright (C) 2018 Luxoft Sweden AB
**
** This file is part of the FaceLift project
**
** Permission is hereby granted, free of charge, to any person
** obtaining a copy of this software and associated documentation files
** (the "Software"), to deal in the Software without restriction,
** including without limitation the rights to use, copy, modify, merge,
** publish, distribute, sublicense, and/or sell copies of the Software,
** and to permit persons to whom the Software is furnished to do so,
** subject to the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
** BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
** ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
**
** SPDX-License-Identifier: MIT
**
*********************************************************************#}

{% set class = '{0}'.format(interface) %}
{%- macro printif(name) -%}
{%- if name -%}{{name}}
{% endif -%}
{%- endmacro -%}

/****************************************************************************
** This is an auto-generated file.
** Do not edit! All changes made to it will be lost.
****************************************************************************/

#pragma once

#include <QtCore>

#include "QMLFrontend.h"
#include "{{class}}.h"

// Dependencies
{% for property in interface.properties -%}
{{- printif(property|requiredInclude) }}
{{- printif(property|requiredQMLInclude) }}
{%- endfor -%}
{% for operation in interface.operations -%}
{% for parameter in operation.parameters -%}
{{- printif(parameter|requiredInclude) }}
{%- endfor %}
{%- endfor %}
{% for event in interface.signals -%}
{% for parameter in event.parameters -%}
{{- printif(parameter|requiredInclude) }}
{%- endfor %}
{%- endfor %}

{{module|namespaceOpen}}

/**
* \class {{class}}QMLFrontend
* \ingroup {{interface.module.name|toValidId}}
* \inqmlmodule {{interface.module.name}}
*/

/**
 * This is the class registered as a QML component for the {{interface}} interface
 */
class {{class}}QMLFrontend : public facelift::QMLFrontendBase
{
    Q_OBJECT

public:

    {{class}}QMLFrontend(QObject* parent = nullptr)
        : facelift::QMLFrontendBase(parent)
    {
    }

    void init({{class}}& provider)
    {
        facelift::QMLFrontendBase::setProvider(provider);
        m_provider = &provider;
        {% for property in interface.properties %}
        connect(m_provider, &{{class}}::{{property.name}}Changed, this, &{{class}}QMLFrontend::{{property.name}}Changed);
        {% if property.type.is_model -%}
        m_{{property}}Model.init(m_provider->{{property}}());
        {% endif %}
        {% endfor %}
        {% for event in interface.signals %}
        {% if event.parameters|hasContainerParameter %}
        connect(m_provider, &{{class}}::{{event.name}}, this, [this] (
            {%- set comma = joiner(", ") -%}
            {%- for parameter in event.parameters -%}
                {{ comma() }}{{parameter|returnType}} {{parameter.name}}
            {%- endfor -%}) {
            emit {{class}}QMLFrontend::{{event.name}}(
            {%- set comma2 = joiner(", ") -%}
            {%- for parameter in event.parameters -%}
                {{comma2()}}
                {%- if parameter.type.is_list or parameter.type.is_map -%}
                    facelift::toQMLCompatibleType({{parameter.name}})
                {%- else -%}
                    {{parameter.name}}
                {%- endif -%}
            {%- endfor -%});
        });
        {% else %}
        connect(m_provider, &{{class}}::{{event.name}}, this, &{{class}}QMLFrontend::{{event.name}});
        {% endif %}
        {% endfor %}
    }

    {% for property in interface.properties %}

    {{- printif(property.comment)}}
    {% if property.type.is_model -%}
    Q_PROPERTY(QObject* {{property}} READ {{property}} NOTIFY {{property.name}}Changed)
    QObject* {{property}}()
    {
        return &m_{{property}}Model;
    }

    facelift::ModelListModel<{{property|nestedType|fullyQualifiedCppName}}> m_{{property}}Model;

    {% elif property.type.is_list or property.type.is_map %}
    // Using {{property|qmlCompatibleType}}, since exposing {{property|returnType}} to QML does not seem to work
    Q_PROPERTY({{property|qmlCompatibleType}} {{property}} READ {{property}}
               {%- if not property.readonly %} WRITE set{{property}}{% endif %} NOTIFY {{property.name}}Changed)
    {{property|qmlCompatibleType}} {{property}}() const
    {
        return facelift::toQMLCompatibleType(m_provider->{{property}}());
    }
        {% if not property.readonly %}
    void set{{property}}(const {{property|qmlCompatibleType}}& newValue)
    {
        // qDebug() << "Request to set property {{property}} to " << newValue;
        Q_ASSERT(m_provider);
        {{property|returnType}} tmp;
        facelift::assignFromQmlType(tmp, newValue);
        m_provider->set{{property}}(tmp);
    }
        {% endif %}
    {%- elif property.type.is_interface %}
    Q_PROPERTY(QObject* {{property}} READ {{property}} NOTIFY {{property.name}}Changed)

    {{property|qmlCompatibleType}} {{property}}()
    {
        return facelift::getQMLFrontend(m_provider->{{property}}());
    }
    {% else %}
        {% if property.readonly %}
    Q_PROPERTY({{property|qmlCompatibleType}} {{property}} READ {{property}} NOTIFY {{property.name}}Changed)
        {%- else %}
    Q_PROPERTY({{property|qmlCompatibleType}} {{property}} READ {{property}} WRITE set{{property}} NOTIFY {{property.name}}Changed)
    void set{{property}}(const {{property|returnType}}& newValue)
    {
        // qDebug() << "Request to set property {{property}} to " << newValue;
        Q_ASSERT(m_provider);
        m_provider->set{{property}}(newValue);
    }
        {%- endif %}

    const {{property|returnType}}& {{property}}() const
    {
        Q_ASSERT(m_provider);
        // qDebug() << "Read property {{property}}. Value: " << m_provider->{{property}}() ;
        return m_provider->{{property}}();
    }
    {% endif %}
    Q_SIGNAL void {{property}}Changed();
    {% endfor %}

    {% for operation in interface.operations %}
    Q_INVOKABLE {{operation|returnType}} {{operation}}(
        {%- set comma = joiner(", ") -%}
        {%- for parameter in operation.parameters -%}
        {{ comma() }}{{parameter|qmlCompatibleType}} {{parameter.name}}
        {%- endfor -%}
    )
    {
        Q_ASSERT(m_provider);
        return m_provider->{{operation}}(
                {%- set comma = joiner(", ") -%}
                {%- for parameter in operation.parameters -%}
                {{ comma() }}
                {%- if parameter|returnType == parameter|qmlCompatibleType -%}
                {{parameter.name}}
                {%- else -%}
                facelift::toProviderCompatibleType<{{parameter|returnType}}, {{parameter|qmlCompatibleType}}>({{parameter.name}})
                {%- endif -%}
                {%- endfor -%}
                );
    }

    {% endfor -%}

    {% for event in interface.signals %}
    Q_SIGNAL void {{event}}(
        {%- set comma = joiner(", ") -%}
        {%- for parameter in event.parameters -%}
        {{ comma() }}{{parameter|qmlCompatibleType}} {{parameter.name}}
        {%- endfor -%}
    );
    {% endfor %}

    QPointer<{{class}}> m_provider;
};


{{module|namespaceClose}}

