using System;
using System.IO;
using System.Text;
using System.Windows.Forms;
using UndertaleModLib.Models;

SaveFileDialog sfd = new SaveFileDialog();
sfd.Filter = "JSON Files|*.json";
sfd.FileName = "room_factory_3_2_collision.json";

if (sfd.ShowDialog() == DialogResult.OK)
{
    StringBuilder json = new StringBuilder();
    json.AppendLine("{");

    UndertaleRoom room = Data.Rooms.ByName("room_bunker2_3");

    if (room != null)
    {
        json.AppendLine($"  \"room_name\": \"{room.Name.Content}\",");
        json.AppendLine($"  \"width\": {room.Width},");
        json.AppendLine($"  \"height\": {room.Height},");
        json.AppendLine("  \"layers\": [");

        for (int l = 0; l < room.Layers.Count; l++)
        {
            var layer = room.Layers[l];
            json.AppendLine("    {");
            json.AppendLine($"      \"layer_name\": \"{layer.LayerName.Content}\",");
            json.AppendLine("      \"instances\": [");

            // GameMaker Studio 2 Layer Instance 데이터 접근
            if (layer.InstancesData != null && layer.InstancesData.Instances != null)
            {
                var instances = layer.InstancesData.Instances;
                for (int i = 0; i < instances.Count; i++)
                {
                    var inst = instances[i];
                    string objName = inst.ObjectDefinition?.Name?.Content ?? "unknown";
                    
                    // XScale / YScale 또는 ScaleX / ScaleY 대응
                    float scaleX = 1.0f;
                    float scaleY = 1.0f;

                    // 동적(dynamic) 타입을 이용해 Scalex, ScaleX, XScale 중 존재하는 프로퍼티 감지
                    dynamic dynamicInst = inst;
                    try { scaleX = (float)dynamicInst.ScaleX; } 
                    catch { try { scaleX = (float)dynamicInst.Scalex; } catch { try { scaleX = (float)dynamicInst.XScale; } catch {} } }

                    try { scaleY = (float)dynamicInst.ScaleY; } 
                    catch { try { scaleY = (float)dynamicInst.Scaley; } catch { try { scaleY = (float)dynamicInst.YScale; } catch {} } }

                    json.Append("        { ");
                    json.Append($"\"object_name\": \"{objName}\", ");
                    json.Append($"\"x\": {inst.X}, ");
                    json.Append($"\"y\": {inst.Y}, ");
                    json.Append($"\"scale_x\": {scaleX}, ");
                    json.Append($"\"scale_y\": {scaleY} ");
                    json.Append("}");

                    if (i < instances.Count - 1) json.Append(",");
                    json.AppendLine();
                }
            }

            json.AppendLine("      ]");
            json.Append("    }");
            if (l < room.Layers.Count - 1) json.Append(",");
            json.AppendLine();
        }

        json.AppendLine("  ]");
    }

    json.AppendLine("}");

    File.WriteAllText(sfd.FileName, json.ToString());
    MessageBox.Show("추출 완료: " + sfd.FileName);
}