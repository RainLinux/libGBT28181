#include "XMLUtility.h"

xmlDocPtr find_element(xmlDocPtr xml_document_pointer, char* element_name)
{
    if(NULL != xml_document_pointer &&
       NULL != element_name)
    {
        while(true)
        {
            if(NULL == xml_document_pointer)
            {
                break;
            }
            if(0 == xmlStrncmp(xml_document_pointer->name, element_name, xmlStrlen(element_name)))
            {
                return (xmlDocPtr)xml_document_pointer;
            }
            xml_document_pointer = (xmlDocPtr)xml_document_pointer->next;
        }
    }
    return NULL;
}

MANSCDP_xml_type get_xml_type(char* xml_type_string)
{
    if(NULL == xml_type_string)
    {
        return MANSCDP_xml_Unknown;
    }

    if(0 == xmlStrncmp(xml_type_string, "Control", xmlStrlen("Control")))
    {
        return MANSCDP_xml_Control;
    }
    if(0 == xmlStrncmp(xml_type_string, "Query", xmlStrlen("Query")))
    {
        return MANSCDP_xml_Query;
    }
    if(0 == xmlStrncmp(xml_type_string, "Notify", xmlStrlen("Notify")))
    {
        return MANSCDP_xml_Notify;
    }
    if(0 == xmlStrncmp(xml_type_string, "Response", xmlStrlen("Response")))
    {
        return MANSCDP_xml_Response;
    }

    return MANSCDP_xml_Unknown;
}

MANSCDP_command_type get_MANSCDP_command_type(char* MANSCDP_command_string)
{
    if(NULL == MANSCDP_command_string)
    {
        return MANSCDP_command_type_unknown;
    }

    if(0 == xmlStrncmp(MANSCDP_command_string, "DeviceInfo", xmlStrlen("DeviceInfo")))
    {
        return MANSCDP_DeviceInfo;
    }
    if(0 == xmlStrncmp(MANSCDP_command_string, "DeviceStatus", xmlStrlen("DeviceStatus")))
    {
        return MANSCDP_DeviceStatus;
    }
    if(0 == xmlStrncmp(MANSCDP_command_string, "Catalog", xmlStrlen("Catalog")))
    {
        return MANSCDP_Catalog;
    }

    return MANSCDP_command_type_unknown;
}

MANSCDP_on_off_line get_MANSCDP_online(char* MANSCDP_string)
{
    if(0 == xmlStrncmp(MANSCDP_string, "ONLINE", xmlStrlen("ONLINE")))
    {
        return MANSCDP_ONLINE;
    }
    if(0 == xmlStrncmp(MANSCDP_string, "OFFLINE", xmlStrlen("OFFLINE")))
    {
        return MANSCDP_OFFLINE;
    }

    return MANSCDP_UNKNOWN_ON_OFF_LINE;
}

MANSCDP_result_type get_MANSCDP_statues(char* MANSCDP_string)
{
    if(0 == xmlStrncmp(MANSCDP_string, "OK", xmlStrlen("OK")))
    {
        return MANSCDP_OK;
    }
    if(0 == xmlStrncmp(MANSCDP_string, "ERROR", xmlStrlen("ERROR")))
    {
        return MANSCDP_ERROR;
    }

    return MANSCDP_UNKNOWN_RESULT_TYPE;
}

void free_MANSCDP_xml_pointer(MANSCDP_xml** pointer)
{
    if(NULL != pointer || NULL != (*pointer))
    {
        osip_free((*pointer)->DeviceID);
        osip_free((*pointer)->Result);
        osip_free((*pointer)->DeviceType);
        osip_free((*pointer)->Manufacturer);
        osip_free((*pointer)->Model);
        osip_free((*pointer)->Firmware);
        for(uint64_t i = 0; i < (*pointer)->catalog_sum_num; i++)
        {
            osip_free((*pointer)->p_MANSCDP_device[i].DeviceID);
            osip_free((*pointer)->p_MANSCDP_device[i].Name);
            osip_free((*pointer)->p_MANSCDP_device[i].Manufacturer);
            osip_free((*pointer)->p_MANSCDP_device[i].Model);
            osip_free((*pointer)->p_MANSCDP_device[i].Owner);
            osip_free((*pointer)->p_MANSCDP_device[i].CivilCode);
            osip_free((*pointer)->p_MANSCDP_device[i].Address);
            osip_free((*pointer)->p_MANSCDP_device[i].Parental);
            osip_free((*pointer)->p_MANSCDP_device[i].SafetyWay);
            osip_free((*pointer)->p_MANSCDP_device[i].RegisterWay);
            osip_free((*pointer)->p_MANSCDP_device[i].Secrecy);
            osip_free((*pointer)->p_MANSCDP_device[i].Status);
        }
        osip_free((*pointer)->p_MANSCDP_device);
        // do not free p_client_configurations
        //osip_free((*pointer)->p_client_configurations);
    }

    osip_free(*pointer);
}

void parse_MANSCDP_xml_device_list(xmlDocPtr xml_device_list, uint64_t count, MANSCDP_device* p_MANSCDP_device)
{
    xml_device_list = (xmlDocPtr)xml_device_list->children;
    xmlDocPtr xmlDocPtr_temp = NULL;
    if(NULL != xml_device_list)
    {
        for(uint64_t i = 0; i < count; i++)
        {
            while(0 != xmlStrncmp(xml_device_list->name, "Item", xmlStrlen("Item")))
            {
                xml_device_list = (xmlDocPtr)xml_device_list->next;
                if(NULL == xml_device_list)
                {
                    break;
                }
            }
            xmlDocPtr_temp = find_element((xmlDocPtr)xml_device_list->children, "DeviceID");
            if(NULL != xmlDocPtr_temp)
            {
                p_MANSCDP_device[i].DeviceID = osip_strdup(xmlDocPtr_temp->children->content);
            }
            xmlDocPtr_temp = find_element((xmlDocPtr)xml_device_list->children, "Name");
            if(NULL != xmlDocPtr_temp)
            {
                p_MANSCDP_device[i].Name = osip_strdup(xmlDocPtr_temp->children->content);
            }
            xmlDocPtr_temp = find_element((xmlDocPtr)xml_device_list->children, "Manufacturer");
            if(NULL != xmlDocPtr_temp)
            {
                p_MANSCDP_device[i].Manufacturer = osip_strdup(xmlDocPtr_temp->children->content);
            }
            xmlDocPtr_temp = find_element((xmlDocPtr)xml_device_list->children, "Model");
            if(NULL != xmlDocPtr_temp)
            {
                p_MANSCDP_device[i].Model = osip_strdup(xmlDocPtr_temp->children->content);
            }
            xmlDocPtr_temp = find_element((xmlDocPtr)xml_device_list->children, "Owner");
            if(NULL != xmlDocPtr_temp)
            {
                p_MANSCDP_device[i].Owner = osip_strdup(xmlDocPtr_temp->children->content);
            }
            xmlDocPtr_temp = find_element((xmlDocPtr)xml_device_list->children, "CivilCode");
            if(NULL != xmlDocPtr_temp)
            {
                p_MANSCDP_device[i].CivilCode = osip_strdup(xmlDocPtr_temp->children->content);
            }
            xmlDocPtr_temp = find_element((xmlDocPtr)xml_device_list->children, "Address");
            if(NULL != xmlDocPtr_temp)
            {
                p_MANSCDP_device[i].Address = osip_strdup(xmlDocPtr_temp->children->content);
            }
            xmlDocPtr_temp = find_element((xmlDocPtr)xml_device_list->children, "Parental");
            if(NULL != xmlDocPtr_temp)
            {
                p_MANSCDP_device[i].Parental = osip_strdup(xmlDocPtr_temp->children->content);
            }
            xmlDocPtr_temp = find_element((xmlDocPtr)xml_device_list->children, "SafetyWay");
            if(NULL != xmlDocPtr_temp)
            {
                p_MANSCDP_device[i].SafetyWay = osip_strdup(xmlDocPtr_temp->children->content);
            }
            xmlDocPtr_temp = find_element((xmlDocPtr)xml_device_list->children, "RegisterWay");
            if(NULL != xmlDocPtr_temp)
            {
                p_MANSCDP_device[i].RegisterWay = osip_strdup(xmlDocPtr_temp->children->content);
            }
            xmlDocPtr_temp = find_element((xmlDocPtr)xml_device_list->children, "Secrecy");
            if(NULL != xmlDocPtr_temp)
            {
                p_MANSCDP_device[i].Secrecy = osip_strdup(xmlDocPtr_temp->children->content);
            }
            xmlDocPtr_temp = find_element((xmlDocPtr)xml_device_list->children, "Status");
            if(NULL != xmlDocPtr_temp)
            {
                p_MANSCDP_device[i].Status = osip_strdup(xmlDocPtr_temp->children->content);
            }
        }
    }
}