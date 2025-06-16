
#include "System/GC/GcControlBlock.hpp"
#include "System/Object.h"

namespace System
{
    namespace Memory
    {
        void GcControlBlock::DestroyObject() noexcept
        {
            if (!m_object_destroyed.exchange(true, std::memory_order_acq_rel))
            {
                if (m_object)
                {
                    // Call virtual finalizer if not suppressed
                    if (!m_object->IsFinalizationSuppressed())
                    {
                        try
                        {
                            m_object->Finalize();
                        }
                        catch (...)
                        {
                            // Ignore exceptions in finalizers
                        }
                    }
                    
                    // Call protected cleanup method
                    try
                    {
                        m_object->OnGcDestroy();
                    }
                    catch (...)
                    {
                        // Ignore exceptions in cleanup
                    }
                    
                    // Delete the object
                    delete m_object;
                    m_object = nullptr;
                }
            }
        }
    }
}
